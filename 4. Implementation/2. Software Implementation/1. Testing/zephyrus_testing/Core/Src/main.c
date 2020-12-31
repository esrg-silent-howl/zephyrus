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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tm_stm32_delay.h"
#include "tm_stm32_nrf24l01.h"
#include "tm_stm32_exti.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* Receiver address */
uint8_t TxAddress[] = {
	0xE7,
	0xE7,
	0xE7,
	0xE7,
	0xE7
};
/* My address */
uint8_t MyAddress[] = {
	0x7E,
	0x7E,
	0x7E,
	0x7E,
	0x7E
};

/* Data received and data for send */
uint8_t dataIn[32] = {0};

/* Interrupt pin settings */
#define IRQ_PORT    GPIOG
#define IRQ_PIN     GPIO_PIN_3

/* NRF transmission status */
TM_NRF24L01_Transmit_Status_t transmissionStatus;
TM_NRF24L01_IRQ_t NRF_IRQ;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ZRFMANAGER_STAC_WORDS	32

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//	nrf24l01 nrf1, nrf3;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
void test_nrf24l01() {
	
//	//nrf24l01 nrf1, nrf3;
//	//uint8_t *nrf1_rx, *nrf1_tx;
//	uint8_t *nrf3_tx, *nrf3_rx;
//	uint8_t *tx_buff = (uint8_t*)"HELLO";
//	uint8_t *rx_buff = (uint8_t*)"BYE12";
//	
//		nrf24l01_config config3;
//		
////		config1.data_rate        = NRF_DATA_RATE_1MBPS;
////		config1.tx_power         = NRF_TX_PWR_0dBm;
////		config1.crc_width        = NRF_CRC_WIDTH_1B;
////		config1.addr_width       = NRF_ADDR_WIDTH_5;
////		config1.payload_length   = 5;    // maximum is 32 bytes
////		config1.retransmit_count = 15;   // maximum is 15 times
////		config1.retransmit_delay = 0x0F; // 4000us, LSB:250us
////		config1.rf_channel       = 127;
////		config1.rx_address       = nrf1_rx;
////		config1.tx_address       = nrf1_tx;
////		config1.rx_buffer        = (uint8_t*)&rx_buff;

////		config1.spi         = &hspi1;
////		config1.spi_timeout = 10; // milliseconds
////		config1.ce_port     = GPIOD;
////		config1.ce_pin      = GPIO_PIN_15;
////		config1.irq_port    = GPIOF;
////		config1.irq_pin     = GPIO_PIN_12;
////		config1.csn_port    = GPIOD;
////		config1.csn_pin     = GPIO_PIN_14;
//		
//		
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
//		config3.rx_buffer        = (uint8_t*)&rx_buff;

//		config3.spi         = &hspi3;
//		config3.spi_timeout = 10; // milliseconds
//		
//		config3.ce_port     = GPIOG;
//		config3.ce_pin      = GPIO_PIN_2;
//		
//		config3.irq_port    = GPIOG;
//		config3.irq_pin     = GPIO_PIN_3;
//		
//		config3.csn_port    = GPIOD;
//		config3.csn_pin     = GPIO_PIN_2;

//		nrf_init(&nrf3, &config3);

//		
//	while (1) {
//		
//		//nrf_send_packet(&nrf1, (uint8_t*)&tx_buff);
//		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
//		nrf_receive_packet(&nrf3);
//		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
//		HAL_Delay(500);
//		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
//		
//	}
}

//void enable_delay(void){
//	DWT->CYCCNT = 0;
//	CoreDebug->DEMCR |= 0x01000000;	
//	DWT->CTRL |= 1;
//}
//void delay(uint32_t tick){
//	uint32_t start = DWT->CYCCNT;
//	uint32_t current = 0;
//	
//	do{
//		current = DWT->CYCCNT;
//	} while((current - start) < tick);
//}

//uint8_t captured;

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//	
//	if(GPIO_Pin == GPIO_PIN_3)
//		nrf_irq_handler(&nrf3);
//}



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Interrupt handler */
//void TM_EXTI_Handler(uint16_t GPIO_Pin) {
//	/* Check for proper interrupt pin */
//	if (GPIO_Pin == IRQ_PIN) {
//		/* Read interrupts */
//		TM_NRF24L01_Read_Interrupts(&NRF_IRQ);
//		
//		/* If data is ready on NRF24L01+ */
//		if (NRF_IRQ.F.DataReady) {
//			/* Get data from NRF24L01+ */
//			TM_NRF24L01_GetData(dataIn);
//			
//			/* Start send */
//			//TM_DISCO_LedOn(LED_GREEN);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
//			
//			/* Send it back, NRF goes automatically to TX mode */
//			TM_NRF24L01_Transmit(dataIn);
//			
//			/* Wait for data to be sent */
//			do {
//				/* Wait till sending */
//				transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
//			} while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
//			
//			/* Send done */
//			//TM_DISCO_LedOff(LED_GREEN);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
//			
//			/* Go back to RX mode */
//			TM_NRF24L01_PowerUpRx();		
//		}
//		
//		/* Clear interrupts */
//		TM_NRF24L01_Clear_Interrupts();
//	}
//}
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
  MX_SPI3_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	
//	enable_delay();

//	// Stop TIM2 counter
//	TIM2->CR1 &= ~TIM_CR1_CEN;
//	// Reset TIM2 counter
//	TIM2->CNT = 0;
//	// Enable Capture/Compare 2 interrupt
//	TIM2->DIER |= TIM_IT_CC2;
//		// Enable the Input Capture channel 2
//	TIM2->CCER |= TIM_CCER_CC2E;
//	
//	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
	
//	loop:
//	
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
//	delay(216*20);	// 20us
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
//	
//	while(!captured);
//	captured = 0;
//	cycles = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);
//	distance = CORRECTION_FACTOR * (cycles*1.0/108000000.0)  *340 / 2;
//	distances[index++] = distance;
//	
//	delay(216*10000);	// 10ms

//	if(index < 20)
//		goto loop;
	
//	test_nrf24l01();
//	while(1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	
	/* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
	/* By default 2Mbps data rate and 0dBm output power */
	/* NRF24L01 goes to RX mode by default */
	TM_NRF24L01_Init(15, 32);
	
	/* Set RF settings, Data rate to 2Mbps, Output power to -18dBm */
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
	
	/* Set my address, 5 bytes */
	TM_NRF24L01_SetMyAddress(MyAddress);
	
	/* Set TX address, 5 bytes */
	TM_NRF24L01_SetTxAddress(TxAddress);
	
	/* Enable interrupts for NRF24L01+ IRQ pin */
	TM_EXTI_Attach(IRQ_PORT, IRQ_PIN, TM_EXTI_Trigger_Falling);

	
  while (1)
  {
		
		if (TM_NRF24L01_DataReady()) {
			/* Get data from NRF24L01+ */
			TM_NRF24L01_GetData(dataIn);
			
			/* Start send */
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
			
			/* Send it back, automatically goes to TX mode */
			TM_NRF24L01_Transmit(dataIn);
			
			/* Wait for data to be sent */
			do {
				/* Wait till sending */
				transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
			} while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
			
			/* Send done */
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
			
			/* Go back to RX mode */
			TM_NRF24L01_PowerUpRx();		
		}
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
