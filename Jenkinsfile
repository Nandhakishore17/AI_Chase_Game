pipeline {
    agent any

    environment {
        VS_VCVARS = 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat'
        BUILD_DIR = 'out\\build\\jenkins-validation'
    }

    stages {
        stage('Environment Check') {
            steps {
                bat '''
                    @echo off
                    echo === AI Chase CI Environment ===

                    git --version
                    if errorlevel 1 exit /b 1

                    cmake --version
                    if errorlevel 1 exit /b 1

                    ctest --version
                    if errorlevel 1 exit /b 1

                    call "%VS_VCVARS%"
                    if errorlevel 1 exit /b 1

                    where cl
                    if errorlevel 1 exit /b 1

                    cl
                    if errorlevel 1 exit /b 1
                '''
            }
        }

        stage('Configure C++ Tests') {
            steps {
                bat '''
                    @echo off
                    call "%VS_VCVARS%"
                    if errorlevel 1 exit /b 1

                    cmake -S . -B "%BUILD_DIR%" -G "Visual Studio 18 2026" -DBUILD_TESTING=ON
                    if errorlevel 1 exit /b 1
                '''
            }
        }

        stage('Build C++ Tests') {
            steps {
                bat '''
                    @echo off
                    call "%VS_VCVARS%"
                    if errorlevel 1 exit /b 1

                    cmake --build "%BUILD_DIR%" --config Debug --target GameQualityTests
                    if errorlevel 1 exit /b 1
                '''
            }
        }

        stage('Run C++ Tests') {
            steps {
                bat '''
                    @echo off

                    ctest --test-dir "%BUILD_DIR%" -C Debug --output-on-failure
                    if errorlevel 1 exit /b 1
                '''
            }
        }
    }

    post {
        success {
            echo 'AI Chase C++ validation pipeline PASSED.'
        }

        failure {
            echo 'AI Chase C++ validation pipeline FAILED.'
        }

        always {
            echo 'AI Chase CI run completed.'
        }
    }
}