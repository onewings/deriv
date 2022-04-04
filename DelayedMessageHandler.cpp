
#include "DelayedMessageHandler.h"

DelayedMessageHandler::DelayedMessageHandler()
{

    std::lock_guard<std::mutex> locker(m_mutex);
    if (m_state == state_t::NONE)
    {
        m_state = state_t::RUNNING;
        m_thread = std::thread(&DelayedMessageHandler::ThreadBody, this);
    }
    else
    {
        throw std::logic_error("DelayedMessageHandler already started");
    }
}

DelayedMessageHandler::~DelayedMessageHandler()
{
    TryStop();
}

void DelayedMessageHandler::OnMessage(const std::string& message)
{
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        std::chrono::steady_clock::time_point scheduleTime = std::chrono::steady_clock::now() + std::chrono::minutes(1);
        m_messages.push_back({scheduleTime, message});
    }
    m_cond.notify_all();
}

DelayedMessageHandler& DelayedMessageHandler::SetMessageHandler(MessageHandler handler)
{
    m_handler = handler;
    return (*this);
}

void DelayedMessageHandler::Stop()
{
    if (!TryStop())
    {
        throw std::logic_error("TaskScheduler never started");
    }
}

bool DelayedMessageHandler::TryStop()
{
    std::unique_lock<std::mutex> locker(m_mutex);
    bool performed = m_state != state_t::NONE;

    if (performed)
    {
        if (m_state == state_t::RUNNING)
        {
            m_state = state_t::STOP_REQ;
            m_cond.notify_all();
        }

        while (m_state == state_t::STOP_REQ)
        {
            m_cond.wait(locker);
        }

        if (m_state == state_t::JOINING)
        {
            m_thread.join();
            m_state = state_t::JOINED;
            m_cond.notify_all();
        }
    }

    return performed;
}

void DelayedMessageHandler::ThreadBody()
{
    bool done = false;
    pthread_setname_np(pthread_self(), "DelayedMsg");
    while (!done)
    {
        std::string messageToSend;
        bool execute = false;
        {
            std::unique_lock<std::mutex> locker(m_mutex);

            // Determines what to do right now
            if (m_state != state_t::RUNNING)
            {
                // stop() invoked and process exiting
                done = true;
                m_state = state_t::JOINING;
                m_cond.notify_all();
            }
            else
            {
                if (m_messages.empty())
                {
                    // No message pending. Sleeps until any new notification
                    m_cond.wait(locker);
                }
                else
                {
                    // Get the first message in the list without extracting it
                    auto it = m_messages.front();

                    std::string& message = it.second;
                    const std::chrono::steady_clock::time_point& next_schedule_time = it.first;

                    if (next_schedule_time < std::chrono::steady_clock::now())
                    {
                        // Scheduled time arrived. Executes right now. Execution must happen out of this critical section.
                        messageToSend = std::move(message);
                        execute = true;
                        m_messages.pop_front();
                    }
                    else
                    {
                        // Sleeps until next execution time or some event is notified
                        m_cond.wait_until(locker, next_schedule_time );
                    }
                }
            }
        }

        if (execute)
        {
            try
            {
                m_handler(messageToSend);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error " << e.what() << " executing handler on message " << messageToSend << std::endl;
            }
            catch (...)
            {
                std::cerr << "Error executing handler Generic Error" << std::endl;
            }
        }
    }
}