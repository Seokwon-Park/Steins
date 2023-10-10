# Steins; Engine
![SteinsEngine copy](https://github.com/Seokwon-Park/Steins/assets/22045739/30397492-0569-415f-943d-5cdc11ac2345)

## Getting Started
IDE - Visual Studio 2022   
Test - Windows 10

<ins>**1. Downloading the repository:**</ins>

Clone the repository   
`git clone --recursive https://github.com/Seokwon-Park/Steins`.

If the repository was cloned non-recursively previously, use    
`git submodule update --init` to clone the necessary submodules.

Then update the submodule with    
`git submodule update command`


<ins>**2. Configuring the dependencies:**</ins>

1. Requires the DirectX SDK. Download the DirectX SDK (it's included in the Windows SDK).   
(DirectX SDK를 요구합니다. DirectX SDK를 다운로드 받아주세요. (Windows SDK에 포함되어 있습니다.))   
[Windows SDK link](https://developer.microsoft.com/ko-kr/windows/downloads/sdk-archive/)   
2. After downloading and unzipping the project, the [Win-GenProjects.bat](https://github.com/Seokwon-Park/Steins/blob/master/scripts/Win-GenProjects.bat) script file will get executed automatically, which will then generate a Visual Studio solution file for user's usage.   
(프로젝트를 다운로드하고 압축을 풀면 Win-GenProjects.bat 스크립트 파일이 자동으로 실행되어 사용자가 사용할 수 있는 Visual Studio 솔루션 파일을 생성합니다.)

If changes are made, or if you want to regenerate project files, rerun the [Win-GenProjects.bat](https://github.com/Seokwon-Park/Steins/blob/master/scripts/Win-GenProjects.bat) script file found in `scripts` folder.
