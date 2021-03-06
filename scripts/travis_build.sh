#!/usr/bin/env bash

function build() {
    mkdir cmake-build-${BUILD_TYPE}
    cd cmake-build-${BUILD_TYPE}
    cmd="${DEPS_DIR}/cmake/bin/cmake"
    if [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then cmd="cmake"; fi
    options="-DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_COMPILER=${CC}"
    if [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then options="-DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/${CXX} -DCMAKE_C_COMPILER=/usr/local/opt/llvm/bin/${CC}"; fi
    if [[ "${CODE_COVERAGE}" == "ON" ]] && [[ "${BUILD_TYPE}" == "Debug" ]]; then
        options+=' -DENABLE_COVERAGE=ON'
    elif [[ "${ASAN}" == "ON" ]] && [[ "${BUILD_TYPE}" == "Debug" ]]; then
        options+=' -DUSE_ASAN=ON'
        doctest_upload_name+='-ASAN'
    elif [[ "${TSAN}" == "ON" ]] && [[ "${BUILD_TYPE}" == "Debug" ]]; then
        options+=' -DUSE_TSAN=ON'
        doctest_upload_name+='-TSAN'
    elif [[ "${UBSAN}" == "ON" ]] && [[ "${BUILD_TYPE}" == "Debug" ]]; then
        options+=' -DUSE_UBSAN=ON'
        doctest_upload_name+='-UBSAN'
        if [[ "${TRAVIS_OS_NAME}" == "linux" ]] && [[ "${DEFAULT_COMPILER}" == "gcc" ]]; then options+=' -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=gold'; fi
        if [[ "${TRAVIS_OS_NAME}" == "linux" ]] && [[ "${DEFAULT_COMPILER}" == "clang" ]]; then options+=' -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld'; fi
    elif [[ "${NINJA}" == "ON" ]]; then
        options+=' -GNinja'
        doctest_upload_name+='-Ninja'
    fi

    echo "result -> ${cmd} ${options} ../"
    ${cmd} ${options} ../
    cmake --build . --target mmbot-test --config ${BUILD_TYPE} || travis_terminate 1
    cmake --build . --target mmbot --config ${BUILD_TYPE} || travis_terminate 1
}

function run_test() {
    cd ${TRAVIS_BUILD_DIR}/cmake-build-${BUILD_TYPE}/bin
    for i in *-test; do ./${i} --reporters=xml --out=${i}-result.xml -s || true; done;
}

doctest_upload_name="Doctest [travis-ci ${TRAVIS_OS_NAME}-${CC}-${BUILD_TYPE}"
if [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then doctest_upload_name="Doctest [travis-ci ${TRAVIS_OS_NAME}-${CLANG_VERSION}-${BUILD_TYPE}"; fi

function upload_test() {
    cd ${TRAVIS_BUILD_DIR}/cmake-build-${BUILD_TYPE}/bin
    doctest_upload_name+=']'
    echo "uploading doctest: ${doctest_upload_name}"
    curl https://report.ci/upload.py --output upload.py && python upload.py -n "${doctest_upload_name}" --merge ".*"
}

if [[ "${WILL_COMPILE_CODE}" == "ON" ]]; then build || travis_terminate 1; fi
if [[ "${WILL_COMPILE_CODE}" == "ON" ]]; then run_test || travis_terminate 1; fi
if [[ "${WILL_COMPILE_CODE}" == "ON" ]]; then upload_test || travis_terminate 1; fi