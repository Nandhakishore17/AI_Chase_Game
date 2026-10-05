pipeline {
    agent any

    environment {
        VS_VCVARS = 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat'
        BUILD_DIR = 'out\\build\\jenkins-validation'
        PYTHON_BOOTSTRAP = 'C:\\Users\\ferra\\AppData\\Local\\Programs\\Python\\Python313\\python.exe'
        PYTHON_VENV = '.venv'
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

        stage('Prepare Python Environment') {
            steps {
                bat '''
                    @echo off
                    echo === Preparing Python CI Environment ===

                    "%PYTHON_BOOTSTRAP%" --version
                    if errorlevel 1 exit /b 1

                    if not exist "%PYTHON_VENV%\\Scripts\\python.exe" (
                        "%PYTHON_BOOTSTRAP%" -m venv "%PYTHON_VENV%"
                        if errorlevel 1 exit /b 1
                    )

                    "%PYTHON_VENV%\\Scripts\\python.exe" -m pip install --disable-pip-version-check -r requirements.txt
                    if errorlevel 1 exit /b 1
                '''
            }
        }

        stage('Run Python Contract Tests') {
            steps {
                bat '''
                    @echo off
                    echo === Running Python Validation Contract Tests ===

                    "%PYTHON_VENV%\\Scripts\\python.exe" -m unittest discover -s Automation\\tests -p "test_*.py" -v
                    if errorlevel 1 exit /b 1
                '''
            }
        }

        stage('Prepare Playwright Environment') {
            steps {
                bat '''
                    @echo off
                    echo === Preparing Playwright API Test Environment ===

                    node --version
                    if errorlevel 1 exit /b 1

                    npm.cmd --version
                    if errorlevel 1 exit /b 1

                    npm.cmd ci
                    if errorlevel 1 exit /b 1

                    npx.cmd playwright --version
                    if errorlevel 1 exit /b 1
                '''
            }
        }

        stage('Run Playwright API Tests') {
            steps {
                bat '''
                    @echo off
                    echo === Running Playwright API Automation ===

                    set "PATH=%CD%\\%PYTHON_VENV%\\Scripts;%PATH%"

                    where python
                    if errorlevel 1 exit /b 1

                    python --version
                    if errorlevel 1 exit /b 1

                    npx.cmd playwright test
                    if errorlevel 1 exit /b 1
                '''
            }
        }
    }

    post {
        success {
            echo 'AI Chase validation pipeline PASSED.'
        }

        failure {
            echo 'AI Chase validation pipeline FAILED.'
        }

        always {
            echo 'AI Chase CI run completed.'
        }
    }
}