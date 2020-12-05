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
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nrf24l01.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	nrf24l01 nrf1, nrf3;
	uint8_t *nrf1_rx, *nrf3_rx;
	uint8_t *nrf1_tx, *nrf3_tx;
	uint8_t *tx_buff = (uint8_t*)"HELLO";
	uint8_t *rx_buff = (uint8_t*)"BYE12";
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
  MX_SPI3_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
	
	{
		nrf24l01_config config1, config3;
		
		config1.data_rate        = NRF_DATA_RATE_1MBPS;
		config1.tx_power         = NRF_TX_PWR_0dBm;
		config1.crc_width        = NRF_CRC_WIDTH_1B;
		config1.addr_width       = NRF_ADDR_WIDTH_5;
		config1.payload_length   = 4;    // maximum is 32 bytes
		config1.retransmit_count = 15;   // maximum is 15 times
		config1.retransmit_delay = 0x0F; // 4000us, LSB:250us
		config1.rf_channel       = 0;
		config1.rx_address       = nrf1_rx;
		config1.tx_address       = nrf1_tx;
		config1.rx_buffer        = (uint8_t*)&rx_buff;

		config1.spi         = &hspi1;
		config1.spi_timeout = 10; // milliseconds
		config1.ce_port     = GPIOD;
		config1.ce_pin      = GPIO_PIN_15;
		config1.irq_port    = GPIOF;
		config1.irq_pin     = GPIO_PIN_12;
		config1.csn_port    = GPIOD;
		config1.csn_pin     = GPIO_PIN_14;
		
		
		config3.data_rate        = NRF_DATA_RATE_1MBPS;
		config3.tx_power         = NRF_TX_PWR_0dBm;
		config3.crc_width        = NRF_CRC_WIDTH_1B;
		config3.addr_width       = NRF_ADDR_WIDTH_5;
		config3.payload_length   = 4;    // maximum is 32 bytes
		config3.retransmit_count = 15;   // maximum is 15 times
		config3.retransmit_delay = 0x0F; // 4000us, LSB:250us
		config3.rf_channel       = 0;
		config3.rx_address       = nrf3_rx;
		config3.tx_address       = nrf3_tx;
		config3.rx_buffer        = (uint8_t*)&tx_buff;

		config3.spi         = &hspi3;
		config3.spi_timeout = 10; // milliseconds
		config3.ce_port     = GPIOG;
		config3.ce_pin      = GPIO_PIN_3;
		config3.irq_port    = GPIOG;
		config3.irq_pin     = GPIO_PIN_3;
		config3.csn_port    = GPIOD;
		config3.csn_pin     = GPIO_PIN_2;

		nrf_init(&nrf1, &config1);
		nrf_init(&nrf3, &config3);
	}

	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	while (1)
	{
		nrf_send_packet(&nrf1, (uint8_t*)&tx_buff);
		nrf_receive_packet(&nrf3);
		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
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

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
