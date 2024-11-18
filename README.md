# Project Overview
- https://github.com/cu-ecen-aeld/final-project-sota6640/wiki/Project-Overview-%E2%80%90-Audio-EQ-Driver

# Build Instructions
- `cd build`
- `cmake ..`
- `make`
- Executable should show up in the project root directory

# To see the audio buffer
- Run the `./audioeqdriver` in the project root directory
- In another terminal session run the test mp3 with `ffplay test_mp3/A\ man.mp3`
- In the audioeqdriver terminal session you should see values other than 0 show up while the audio is playing