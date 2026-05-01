# Mini-quadcopter with STM32
Mini quadcopter with STM32 microcontroller, featuring fully bare-metal firmware in C-code (no HAL, no libraries - direct register programming) and control algorithm developed with Matlab/Simulink.

🚧 Project Status: Work in Progress

Pushed on main branch
- [x] Initial implementation of:
      1. Clock and Reset (RCC) (HSI Clock)
      2. General-Purpose Timer (1 Channel on TIM4)
      3. PWM control on TIM4 channel
      4. SPI driver
      5. GPIOs
- [x] Clock source as PLL at 84 MHz.
- [x] SysTick implementation. Extend implementation for TIM4 and PWM (for remaining 3 channels).
- [x] Pressure, Magnetic and Accelerometer/IMU sensors firmware implementation (with related SPI and GPIO configuration).
- [X] PWM update/optimization (variable frequency PWM and switched to center-aligned)
- [X] SPI code improvement after full testing of reading sensors' data

In Progress
- [ ] BLE module configuration
- [ ] PID controllers implementation and tuning

Upcoming / In pipeline
- [ ] Custom makefile (indipendent from STM32CubeIDE internal makefile)
- [ ] Integration of firmware and control algorithm (Simulink generated code)
- [ ] HAL layer creation and firmware code cleanup
