/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tm_stm32_exti.h"
#include "tm_stm32_nrf24l01.h"
#include "tm_stm32_delay.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* My address */
uint8_t MyAddress[] = {
	0xE7,
	0xE7,
	0xE7,
	0xE7,
	0xE7
};
/* Receiver address */
uint8_t TxAddress[] = {
	0x7E,
	0x7E,
	0x7E,
	0x7E,
	0x7E
};

/* Data received and data for send */
uint8_t dataOut[32] = "HELLOSTM32HELLOSTM32hellostm32F0", dataIn[32];

/* Interrupt pin settings */
#define IRQ_PORT    GPIOA
#define IRQ_PIN     GPIO_PIN_4

/* NRF transmission status */
TM_NRF24L01_Transmit_Status_t transmissionStatus;
TM_NRF24L01_IRQ_t NRF_IRQ;

/* Buffer for strings */
char str[40];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//MPU6050_t MPU6050;
//nrf24l01 nrf1, nrf3;
//volatile nrf24l01 nrf1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void test_nrf24l01() {
	
//	uint8_t *nrf1_rx, *nrf1_tx;
//	//uint8_t *nrf3_rx, *nrf3_tx;
//	uint8_t *tx_buff = (uint8_t*)"HELLO";
//	uint8_t *rx_buff = (uint8_t*)"BYE12";
	
		//nrf24l01_config config1, config3;
//		nrf24l01_config config1;
//		
//		config1.data_rate        = NRF_DATA_RATE_1MBPS;
//		config1.tx_power         = NRF_TX_PWR_0dBm;
//		config1.crc_width        = NRF_CRC_WIDTH_1B;
//		config1.addr_width       = NRF_ADDR_WIDTH_5;
//		config1.payload_length   = 5;    // maximum is 32 bytes
//		config1.retransmit_count = 15;   // maximum is 15 times
//		config1.retransmit_delay = 0x0F; // 4000us, LSB:250us
//		config1.rf_channel       = 127;
//		config1.rx_address       = nrf1_rx;
//		config1.tx_address       = nrf1_tx;
//		config1.rx_buffer        = (uint8_t*)&tx_buff;

//		config1.spi         = &hspi1;
//		config1.spi_timeout = 10; // milliseconds
//		
//		config1.ce_port     = GPIOB;
//		config1.ce_pin      = GPIO_PIN_1;
//		
//		config1.irq_port    = GPIOA;
//		config1.irq_pin     = GPIO_PIN_4;
//		
//		config1.csn_port    = GPIOB;
//		config1.csn_pin     = GPIO_PIN_0;
		
		
//		config3.data_rate        = NRF_DATA_RATE_1MBPS;
//		config3.tx_power         = NRF_TX_PWR_0dBm;
//		config3.crc_width        = NRF_CRC_WIDTH_1B;
//		config3.addr_width       = NRF_ADDR_WIDTH_5;
//		config3.payload_length   = 5;    // maximum is 32 bytes
//		config3.retransmit_count = 15;   // maximum is 15 times
//		config3.retransmit_delay = 0x0F; // 4000us, LSB:250us
//		config3.rf_channel       = 127;
//		config3.rx_address       = nrf3_rx;
//		config3.tx_address       = nrf3_tx;
//		config3.rx_buffer        = (uint8_t*)&tx_buff;

//		config3.spi         = &hspi3;
//		config3.spi_timeout = 10; // milliseconds
//		config3.ce_port     = GPIOG;
//		config3.ce_pin      = GPIO_PIN_2;
//		config3.irq_port    = GPIOG;
//		config3.irq_pin     = GPIO_PIN_3;
//		config3.csn_port    = GPIOD;
//		config3.csn_pin     = GPIO_PIN_2;

//		nrf_init((nrf24l01*)&nrf1, &config1);
//		nrf_init(&nrf3, &config3);

		
//	while (1) {
//		
//			nrf_send_packet((nrf24l01*)&nrf1, (uint8_t*)&tx_buff);
//		nrf_receive_packet(&nrf3);
//		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
//		HAL_Delay(500);
//		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
		
//	}
}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//	
//	if(GPIO_Pin == GPIO_PIN_4)
//		nrf_irq_handler((nrf24l01*)&nrf1);
//	
////	if(GPIO_Pin == GPIO_PIN_3)
////		nrf_irq_handler(&nrf3);
//}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void TM_EXTI_Handler(uint16_t GPIO_Pin) {
	/* Check for proper interrupt pin */
	if (GPIO_Pin == IRQ_PIN) {
		/* Read interrupts */
		TM_NRF24L01_Read_Interrupts(&NRF_IRQ);
		
		/* Check if transmitted OK */
		if (NRF_IRQ.F.DataSent) {
			/* Save transmission status */
			transmissionStatus = TM_NRF24L01_Transmit_Status_Ok;
			
			/* Turn off led */
			//TM_DISCO_LedOff(LED_GREEN);
			
			/* Go back to RX mode */
			TM_NRF24L01_PowerUpRx();
		}
		
		/* Check if max retransmission reached and last transmission failed */
		if (NRF_IRQ.F.MaxRT) {
			/* Save transmission status */
			transmissionStatus = TM_NRF24L01_Transmit_Status_Lost;
			
			/* Turn off led */
			//TM_DISCO_LedOff(LED_GREEN);
			
			/* Go back to RX mode */
			TM_NRF24L01_PowerUpRx();
		}
		
		/* If data is ready on NRF24L01+ */
		if (NRF_IRQ.F.DataReady) {
			/* Get data from NRF24L01+ */
			TM_NRF24L01_GetData(dataIn);		
		}
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
//	while (MPU6050_Init(&hi2c1) == 1);

//	// Stop TIM1 counter
//	TIM1->CR1 &= ~TIM_CR1_CEN;
//	// Reset TIM1 counter
//	TIM1->CNT = 0;
//	// Enable Capture/Compare 1 interrupt
//	TIM1->DIER |= TIM_IT_CC1;
//	// Enable the Input Capture channel 1
//	TIM1->CCER |= TIM_CCER_CC1E;
//	
//	HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_2);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//test_nrf24l01();
//while(1);

	  /* Initialize system and Delay functions */
    TM_DELAY_Init();

	/* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
	/* By default 2Mbps data rate and 0dBm output power */
	/* NRF24L01 goes to RX mode by default */
	TM_NRF24L01_Init(15, 32);
	
	/* Set 2MBps data rate and -18dBm output power */
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
	
	/* Set my address, 5 bytes */
	TM_NRF24L01_SetMyAddress(MyAddress);
	
	/* Set TX address, 5 bytes */
	TM_NRF24L01_SetTxAddress(TxAddress);
	
	/* Attach interrupt for NRF IRQ pin */
	TM_EXTI_Attach(IRQ_PORT, IRQ_PIN, TM_EXTI_Trigger_Falling);
	
	/* Reset counter */
	TM_DELAY_SetTime(2001);
	
  while (1)
  {
		
/* Every 2 seconds */
		if (TM_DELAY_Time() > 2000) {
			
			/* Fill data with something */
			//sprintf((char *)dataOut, "abcdefghijklmnoszxABCDEFCBDA");
			
			/* Display on USART */
			//TM_USART_Puts(USART1, "pinging: ");
			
			/* Reset time, start counting microseconds */
			TM_DELAY_SetTime(0);
			
			/* Transmit data, goes automatically to TX mode */
			TM_NRF24L01_Transmit(dataOut);
			
			/* Turn on led to indicate sending */
			//TM_DISCO_LedOn(LED_GREEN);
			
			/* Wait for data to be sent */
			do {
				/* Get transmission status */
				transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
			} while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
			
			/* Turn off led */
			//TM_DISCO_LedOff(LED_GREEN);
			
			/* Go back to RX mode */
			TM_NRF24L01_PowerUpRx();
			
			/* Wait received data, wait max 100ms, if time is larger, then data were probably lost */
			while (!TM_NRF24L01_DataReady() && TM_DELAY_Time() < 100);
			
			/* Format time */
			//sprintf(str, "%d ms", TM_DELAY_Time());
			
			/* Show ping time */
			//TM_USART_Puts(USART1, str);
			
			/* Get data from NRF2L01+ */
			TM_NRF24L01_GetData(dataIn);
			
			/* Check transmit status */
			if (transmissionStatus == TM_NRF24L01_Transmit_Status_Ok) {
				/* Transmit went OK */
				//TM_USART_Puts(USART1, ": OK\n");
			} else if (transmissionStatus == TM_NRF24L01_Transmit_Status_Lost) {
				/* Message was LOST */
				//TM_USART_Puts(USART1, ": LOST\n");
			} else {
				/* This should never happen */
				//TM_USART_Puts(USART1, ": SENDING\n");
			}
		}
		
//		MPU6050_Read_All(&hi2c1, &MPU6050);
//	  HAL_Delay (100);
//		printf("Kalman X value: %f\n", MPU6050.KalmanAngleX);
//		printf("Kalman Y value: %f\n", MPU6050.KalmanAngleY);
//		printf("-------------------------------------- \n");
//		HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
