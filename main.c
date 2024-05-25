/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
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
#include "software_timer.h"
#include "scheduler.h"
#include "fsm_smart_irrigation.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
void display7SEG(int count) {
	switch (count) {
	case 0:
		HAL_GPIO_WritePin(GPIOB,
		SEG0_Pin | SEG1_Pin | SEG2_Pin | SEG3_Pin | SEG4_Pin | SEG5_Pin,
				GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG6_Pin, GPIO_PIN_SET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, SEG1_Pin | SEG2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,
		SEG0_Pin | SEG3_Pin | SEG4_Pin | SEG5_Pin | SEG6_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB,
		SEG0_Pin | SEG1_Pin | SEG6_Pin | SEG4_Pin | SEG3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG2_Pin | SEG5_Pin, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOB,
		SEG0_Pin | SEG1_Pin | SEG6_Pin | SEG2_Pin | SEG3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG4_Pin | SEG5_Pin, GPIO_PIN_SET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOB, SEG1_Pin | SEG2_Pin | SEG5_Pin | SEG6_Pin,
				GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG0_Pin | SEG3_Pin | SEG4_Pin, GPIO_PIN_SET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOB,
		SEG0_Pin | SEG2_Pin | SEG3_Pin | SEG5_Pin | SEG6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG1_Pin | SEG4_Pin, GPIO_PIN_SET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOB,
		SEG0_Pin | SEG2_Pin | SEG3_Pin | SEG4_Pin | SEG5_Pin | SEG6_Pin,
				GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG1_Pin, GPIO_PIN_SET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOB, SEG0_Pin | SEG1_Pin | SEG2_Pin,
				GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG3_Pin | SEG4_Pin | SEG5_Pin | SEG6_Pin,
				GPIO_PIN_SET);
		break;
	case 8:
		HAL_GPIO_WritePin(GPIOB,
				SEG0_Pin | SEG1_Pin | SEG2_Pin | SEG3_Pin | SEG4_Pin | SEG5_Pin
						| SEG6_Pin, GPIO_PIN_RESET);
		break;
	case 9:
		HAL_GPIO_WritePin(GPIOB,
		SEG0_Pin | SEG1_Pin | SEG2_Pin | SEG3_Pin | SEG5_Pin | SEG6_Pin,
				GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, SEG4_Pin, GPIO_PIN_SET);
		break;
	default:
		HAL_GPIO_WritePin(GPIOB,
				SEG0_Pin | SEG1_Pin | SEG2_Pin | SEG3_Pin | SEG4_Pin | SEG5_Pin
						| SEG6_Pin, GPIO_PIN_SET);
		break;
	}
}

const int MAX_LED_MATRIX = 4;
int index_led = 0;
int led_buffer[4] = { 1, 2, 3, 4 };

void update7SEG(int index) {
	switch (index) {
	case 0:
		display7SEG(led_buffer[0]);
		HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, EN1_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_SET);
		break;
	case 1:
		display7SEG(led_buffer[1]);
		HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_SET);
		break;
	case 2:
		display7SEG(led_buffer[2]);
		HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin | EN3_Pin, GPIO_PIN_SET);
		break;
	case 3:
		display7SEG(led_buffer[3]);
		HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin | EN2_Pin, GPIO_PIN_SET);
		break;
	default:
		break;
	}
}

void updateClockBuffer(int hour, int minute) {
	if (hour / 10 == 0) {
		led_buffer[0] = 0;
		led_buffer[1] = hour;
	} else {
		led_buffer[0] = hour / 10;
		led_buffer[1] = hour % 10;
	}

	if (minute / 10 == 0) {
		led_buffer[2] = 0;
		led_buffer[3] = minute;
	} else {
		led_buffer[2] = minute / 10;
		led_buffer[3] = minute % 10;
	}
}

// Define task functions
void task_LED_Blink(void) {
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}
typedef struct {
	uint32_t period;
	uint32_t lastRunTime;
	void (*taskFunc)(void);
} Task;



// The current task being executed
int currentTask = 0;

void task_T1(void) {
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}
void task_T2(void) {
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}
void task_T3(void) {
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}
void task_T4(void) {
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}
void task_T5(void) {
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}
// Create tasks for T1 to T5
Task tasks[] = { { 2000, 0, task_T1 },  // T1 will run every 2 seconds
		{ 2000, 0, task_T2 },  // T2 will run every 2 seconds
		{ 2000, 0, task_T3 },  // T3 will run every 2 seconds
		{ 5000, 0, task_T4 },  // T4 will run every 5 seconds
		{ 10000, 0, task_T5 }  // T5 will run every 10 seconds
};
const int NUM_TASKS = sizeof(tasks) / sizeof(Task);
void task_Double_Dots(void) {
	HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
}

void task_Update_Time(void) {
	static int hour = 2, minute = 10, second = 50;

	second++;
	if (second > 60) {
		second = 0;
		minute++;
	}

	if (minute >= 60) {
		minute = 0;
		hour++;
	}

	if (hour >= 24) {
		hour = 0;
	}

	updateClockBuffer(hour, minute);
}

void task_Update_7SEG(void) {
	static int index = 0;
	update7SEG(index++);
	if (index > 3) {
		index = 0;
	}
}

void task_Update_LED_Matrix(void) {
	// Implement LED matrix update task
}

void task_LED_Matrix_Animation(void) {
	// Implement LED matrix animation task
}

int main(void) {
	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM2_Init();

	/* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	/* USER CODE END 2 */

	/* Scheduler Initialization */
	SCH_Init();

    fsm_irrigation_init();
    SCH_Add_Task(timerRun,1,1);
    SCH_Add_Task(fsm_irrigation_run,5,1);
//	/* Calculate start delays based on sequence timings */
//	uint32_t t1_start_delay = 0;        // T1 starts immediately
//	uint32_t t2_start_delay = 2000;     // T2 starts 2 seconds after T1
//	uint32_t t3_start_delay = 4000;     // T3 starts 2 seconds after T2
//	uint32_t t4_start_delay = 6000;     // T4 starts 2 seconds after T3
//	uint32_t t5_start_delay = 11000;    // T5 starts 5 seconds after T4
//
//	/* Adding tasks to the scheduler */
//	SCH_Add_Task(task_T1, t1_start_delay, 60000); // LED T1 blinks at the beginning of each cycle
//	SCH_Add_Task(task_T2, t2_start_delay, 60000); // LED T2 blinks 2 seconds into the cycle
//	SCH_Add_Task(task_T3, t3_start_delay, 60000); // LED T3 blinks 4 seconds into the cycle
//	SCH_Add_Task(task_T4, t4_start_delay, 60000); // LED T4 blinks 6 seconds into the cycle
//	SCH_Add_Task(task_T5, t5_start_delay, 60000); // LED T5 blinks 11 seconds into the cycle

	/* Optional tasks */
	// SCH_Add_Task(task_Update_LED_Matrix, 0, 250); // LED Matrix update every 250ms
	// SCH_Add_Task(task_LED_Matrix_Animation, 0, 500); // LED Matrix animation every 500ms
	/* Main loop */
	while (1) {
		SCH_Dispatch_Tasks(); // Run due tasks
		SCH_Update(); // Update task counters and flags
		HAL_Delay(10);
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	timerRun();
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 7999;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 9;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LED_RED_Pin | EN0_Pin | EN1_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
	SEG0_Pin | SEG1_Pin | SEG2_Pin | SEG3_Pin | SEG4_Pin | SEG5_Pin | SEG6_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pins : LED_RED_Pin EN0_Pin EN1_Pin */
	GPIO_InitStruct.Pin = LED_RED_Pin | EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin
			| DOT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : SEG0_Pin SEG1_Pin SEG2_Pin SEG3_Pin
	 SEG4_Pin SEG5_Pin SEG6_Pin */
	GPIO_InitStruct.Pin = SEG0_Pin | SEG1_Pin | SEG2_Pin | SEG3_Pin | SEG4_Pin
			| SEG5_Pin | SEG6_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
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
		 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	  /* USER CODE END 6 */
	}
	#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
