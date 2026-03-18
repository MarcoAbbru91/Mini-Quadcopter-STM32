# Mini-quadcopter with STM32
Mini quadcopter with STM32 microcontroller, featuring bare-metal firmware in C-code (no HAL, no libraries - direct register programming) and control algorithm with Matlab/Simulink.

🚧 Project Status: Work in Progress

Initial analysis and setup
- [x] Hardware platform analysis
- [x] Bare-metal STM32 firmware structure
- [x] Quadcopter plant model analysis

Committed on main branch
- [x] Clock and Reset (RCC) initial implementation (HSI Clock)
- [x] General-Purpose Timer initial implementation (1 Channel on TIM4)
- [x] PWM control on TIM4 channel
- [x] SPI driver initial implementation
- [x] GPIO initial implementation
- [x] Clock source as PLL at 84 MHz
- [x] Extend implementation for TIM4 and PWM. SysTick implementation

In Progress
- [ ] Pressure, Magnetic and Accelerometer/IMU sensors firmware implementation (with related SPI and GPIO configuration)
- [ ] SPI code fixes
- [ ] Make PWM have variable frequency (runtime duty control below)
- [ ] Switch to center-aligned PWM (better for motors) 

Upcoming / In pipeline
- [ ] Mutex/semaphore implementation since SPI is shared by multiple tasks 
- [ ] BLE module configuration
- [ ] Custome makefile (indipendent from STM32CubeIDE internal makefile)
- [ ] PID controllers implementation
- [ ] Integration of Firmware and control algorithm

Future ToDos for improvements
- [ ] Implement LQR as control algorithm

