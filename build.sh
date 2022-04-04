#!/bin/bash
# build helper for Deriv + docker image, push it to docker registry
WORKDIR="${PWD}"
HF_VERSION="v1.0"
GIT_TAG="$(git -C ${WORKDIR} rev-parse --short HEAD)"
BUILD_COMMAND="docker build  --build-arg HF_TAG=${GIT_TAG} --build-arg RELEASE=${HF_VERSION} --network=host --file ${WORKDIR}/docker"

# run or create Deriv compiler
docker start -ai deriv-compiler 2>&1 | tee /tmp/compiler-output
if [ ${PIPESTATUS[0]} -ne 0 ]; then
    grep "No such container" /tmp/compiler-output >/dev/null
    if [ $? -ne 0 ]; then
        exit 1
    else
        echo ${BUILD_COMMAND}/deriv-base-dockerfile --tag deriv-base:${HF_VERSION} ${WORKDIR}/docker
        ${BUILD_COMMAND}/deriv-base-dockerfile --tag deriv-base:${HF_VERSION} ${WORKDIR}/docker
        on_error build failed
        echo ${BUILD_COMMAND}/deriv-compiler-dockerfile --tag deriv-compiler:${HF_VERSION} ${WORKDIR}/docker
        ${BUILD_COMMAND}/deriv-compiler-dockerfile --tag deriv-compiler:${HF_VERSION} ${WORKDIR}/docker
        on_error build failed
        docker run --rm --network=host -it --volume "${WORKDIR}:/root/deriv" --name deriv-compiler deriv-compiler:${HF_VERSION}
        on_error compile failed
    fi
fi