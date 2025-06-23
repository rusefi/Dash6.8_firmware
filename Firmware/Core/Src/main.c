/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "cmsis_os.h"
#include "app_touchgfx.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include <File_Handling.h>
#include <stdio.h>
#include "Globals.h"
#include "extern.h"
#include "sdram.h"

#include "nmea.h"
#include "TargetTouch.h"
#include "Setup/Dash/setupDash.h"
#include "Setup/Field/setupField.h"

#include "eeprom24lc512.h"

#include <math.h>
#include "usart3_json_dma.h"
#include "cJSON.h"

#include"CAN.h"

//#include "z_flash_W25QXXX.h"
#include "level_tables.h"
#include "commands.h"
#include "data.h"
#include "json_dispatcher.h"
#include "W25Qxx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */








osMessageQueueId_t jsonQueueHandle;
nmea_t gps;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define JSON_MSG_MAX_LEN 512
#define RPM_THRESHOLD 500
#define RPM_HYSTERESIS 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//#define LCD_ORIENTATION_LANDSCAPE 0x01
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan1;

CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

LTDC_HandleTypeDef hltdc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim13;

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

SDRAM_HandleTypeDef hsdram1;

/* Definitions for START_Task */
osThreadId_t START_TaskHandle;
const osThreadAttr_t START_Task_attributes = {
  .name = "START_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TouchGFXTask */
osThreadId_t TouchGFXTaskHandle;
const osThreadAttr_t TouchGFXTask_attributes = {
  .name = "TouchGFXTask",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for BRIGHTNESS_Task */
osThreadId_t BRIGHTNESS_TaskHandle;
const osThreadAttr_t BRIGHTNESS_Task_attributes = {
  .name = "BRIGHTNESS_Task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for nmea */
osThreadId_t nmeaHandle;
const osThreadAttr_t nmea_attributes = {
  .name = "nmea",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Ind_OIL */
osThreadId_t Ind_OILHandle;
const osThreadAttr_t Ind_OIL_attributes = {
  .name = "Ind_OIL",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Ind_FUEL */
osThreadId_t Ind_FUELHandle;
const osThreadAttr_t Ind_FUEL_attributes = {
  .name = "Ind_FUEL",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Ind_BATT */
osThreadId_t Ind_BATTHandle;
const osThreadAttr_t Ind_BATT_attributes = {
  .name = "Ind_BATT",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Ind_ECT */
osThreadId_t Ind_ECTHandle;
const osThreadAttr_t Ind_ECT_attributes = {
  .name = "Ind_ECT",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for other */
osThreadId_t otherHandle;
const osThreadAttr_t other_attributes = {
  .name = "other",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for RPM_10 */
osThreadId_t RPM_10Handle;
const osThreadAttr_t RPM_10_attributes = {
  .name = "RPM_10",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for FUELUSED_11 */
osThreadId_t FUELUSED_11Handle;
const osThreadAttr_t FUELUSED_11_attributes = {
  .name = "FUELUSED_11",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UART_Task */
osThreadId_t UART_TaskHandle;
const osThreadAttr_t UART_Task_attributes = {
  .name = "UART_Task",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for MotohoursTask */
osThreadId_t MotohoursTaskHandle;
const osThreadAttr_t MotohoursTask_attributes = {
  .name = "MotohoursTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Button_R_Task */
osThreadId_t Button_R_TaskHandle;
const osThreadAttr_t Button_R_Task_attributes = {
  .name = "Button_R_Task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Button_L_Task */
osThreadId_t Button_L_TaskHandle;
const osThreadAttr_t Button_L_Task_attributes = {
  .name = "Button_L_Task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for JSON_Parse_Task */
osThreadId_t JSON_Parse_TaskHandle;
const osThreadAttr_t JSON_Parse_Task_attributes = {
  .name = "JSON_Parse_Task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for jsonQueue */
osMessageQueueId_t jsonQueueHandle;
const osMessageQueueAttr_t jsonQueue_attributes = {
  .name = "jsonQueue"
};
/* USER CODE BEGIN PV */
FMC_SDRAM_CommandTypeDef command;

Statuses Current_Status;
FlagsUnion_t flags_union;
Flags_t device_flags;

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];

uint8_t uartTransmitBufferSize;
uint8_t uartTransmitBuffer[128];

static char json_buf[JSON_MSG_MAX_LEN];

int rpmWindowSize = 10;     //  размер окна для RPM
int windows[2] = {10, 30};   //  0 fuelFlow  1

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_LTDC_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM13_Init(void);
static void MX_CAN1_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C3_Init(void);
void Start_START_Task(void *argument);
void TouchGFX_Task(void *argument);
void Start_BRIGHTNESS_Task(void *argument);
void Start_nmea(void *argument);
void Start_OIL(void *argument);
void Start_Ind_FUEL(void *argument);
void Start_IndBATT(void *argument);
void Start_Ind_ECT(void *argument);
void Start_other(void *argument);
void Start_RMP_10(void *argument);
void Start_FUELUSED_11(void *argument);
void Start_UART_Task(void *argument);
void Start_MotohoursTask(void *argument);
void Start_R_Buttan_Task14(void *argument);
void Start_Buttan_L_Task(void *argument);
void Start_JSON_Parse_Task(void *argument);

/* USER CODE BEGIN PFP */
Flags_t can_k;
EEPROM_Config_t eeprom_cfg;
void HAL_UART_TxCpltCallback();
void initAll(void);
bool LoadFlagsFromEEPROM(void);
void SaveFlagsToEEPROM(void);
void InitFlags(void);
void app_init_usart3(void);
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
  MX_DMA_Init();
  MX_LTDC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_CRC_Init();
  MX_TIM13_Init();
  MX_CAN1_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  W25qxx_Init();
  MX_USART3_UART_Init();
  MX_I2C3_Init();
  MX_TouchGFX_Init();
  /* Call PreOsInit function */
  MX_TouchGFX_PreOSInit();
  /* USER CODE BEGIN 2 */
  // W25qxx_Init();
  InitFlags();
  EEPROM_Config_t eeprom_cfg = {
          .hi2c = &hi2c3,
          .hcrc = 0,
          .i2c_address = 0x50 << 1,
          .page_size = 128,
          .write_timeout = 100,
          .read_timeout = 50,
          .motohours_main_addr = 0x0000,
          .motohours_backup_addr = 0x0100,
          .flags_main_addr = 0x0200,
          .flags_backup_addr = 0x0300,
          .array_main_addr = 0x0400,
          .array_backup_addr = 0x0500
      };
  app_init_usart3();
  EEPROM_Init(&eeprom_cfg);
//  LoadFlagsFromEEPROM();
  initAll();


 // USART3_JSON_DMA_Init(115200);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of jsonQueue */
  jsonQueueHandle = osMessageQueueNew (6, 512, &jsonQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */

	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of START_Task */
  START_TaskHandle = osThreadNew(Start_START_Task, NULL, &START_Task_attributes);

  /* creation of TouchGFXTask */
  TouchGFXTaskHandle = osThreadNew(TouchGFX_Task, NULL, &TouchGFXTask_attributes);

  /* creation of BRIGHTNESS_Task */
  BRIGHTNESS_TaskHandle = osThreadNew(Start_BRIGHTNESS_Task, NULL, &BRIGHTNESS_Task_attributes);

  /* creation of nmea */
  nmeaHandle = osThreadNew(Start_nmea, NULL, &nmea_attributes);

  /* creation of Ind_OIL */
  Ind_OILHandle = osThreadNew(Start_OIL, NULL, &Ind_OIL_attributes);

  /* creation of Ind_FUEL */
  Ind_FUELHandle = osThreadNew(Start_Ind_FUEL, NULL, &Ind_FUEL_attributes);

  /* creation of Ind_BATT */
  Ind_BATTHandle = osThreadNew(Start_IndBATT, NULL, &Ind_BATT_attributes);

  /* creation of Ind_ECT */
  Ind_ECTHandle = osThreadNew(Start_Ind_ECT, NULL, &Ind_ECT_attributes);

  /* creation of other */
  otherHandle = osThreadNew(Start_other, NULL, &other_attributes);

  /* creation of RPM_10 */
  RPM_10Handle = osThreadNew(Start_RMP_10, (void*) &rpmWindowSize, &RPM_10_attributes);

  /* creation of FUELUSED_11 */
  FUELUSED_11Handle = osThreadNew(Start_FUELUSED_11, (void*) &windows, &FUELUSED_11_attributes);

  /* creation of UART_Task */
  UART_TaskHandle = osThreadNew(Start_UART_Task, NULL, &UART_Task_attributes);

  /* creation of MotohoursTask */
  MotohoursTaskHandle = osThreadNew(Start_MotohoursTask, NULL, &MotohoursTask_attributes);

  /* creation of Button_R_Task */
  Button_R_TaskHandle = osThreadNew(Start_R_Buttan_Task14, NULL, &Button_R_Task_attributes);

  /* creation of Button_L_Task */
  Button_L_TaskHandle = osThreadNew(Start_Buttan_L_Task, NULL, &Button_L_Task_attributes);

  /* creation of JSON_Parse_Task */
  JSON_Parse_TaskHandle = osThreadNew(Start_JSON_Parse_Task, NULL, &JSON_Parse_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */


  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	while (1) {
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

	CAN_FilterTypeDef sFilterConfig; //declare CAN filter structure
  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 12;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
	sFilterConfig.FilterBank = 15;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	//sFilterConfig.SlaveStartFilterBank = 14;
	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
		/* Filter configuration Error */
		Error_Handler();
	}
	if (HAL_CAN_Start(&hcan1) != HAL_OK) {
		/* Start Error */
		Error_Handler();
	}
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING)
			!= HAL_OK) {
		Error_Handler();
	}
	__HAL_RCC_CAN1_CLK_ENABLE();
  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 52;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AH;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AH;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IIPC;
  hltdc.Init.HorizontalSync = 5;
  hltdc.Init.VerticalSync = 14;
  hltdc.Init.AccumulatedHBP = 17;
  hltdc.Init.AccumulatedVBP = 44;
  hltdc.Init.AccumulatedActiveW = 497;
  hltdc.Init.AccumulatedActiveH = 1324;
  hltdc.Init.TotalWidth = 509;
  hltdc.Init.TotalHeigh = 1339;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 1280;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0xD0000000;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 1280;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 210;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM13 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM13_Init(void)
{

  /* USER CODE BEGIN TIM13_Init 0 */

  /* USER CODE END TIM13_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM13_Init 1 */

  /* USER CODE END TIM13_Init 1 */
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 90-1;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 1000-1;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim13, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM13_Init 2 */

  /* USER CODE END TIM13_Init 2 */
  HAL_TIM_MspPostInit(&htim13);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 9600;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 8;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 5;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
	FMC_SDRAM_CommandTypeDef command;
	if (SDRAM_Initialization_Sequence(&hsdram1, &command) != HAL_OK) {
		Error_Handler();
	}
  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED_CAN2_Pin|LED_CAN1_Pin|LED_ALERT_Pin|LED_ACTIVE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TOUCH_RST_PC13_GPIO_Port, TOUCH_RST_PC13_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, CAN1_S0_Pin|CAN2_S0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_CS_PIN_GPIO_Port, SD_CS_PIN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, IN_S0_Pin|IN_S1_Pin|IN_S2_Pin|PUD_E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, IN_E_Pin|OUT_S0_Pin|PUD_S2_Pin|PUD_S1_Pin
                          |PUD_S0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED_CAN2_Pin LED_CAN1_Pin LED_ALERT_Pin LED_ACTIVE_Pin */
  GPIO_InitStruct.Pin = LED_CAN2_Pin|LED_CAN1_Pin|LED_ALERT_Pin|LED_ACTIVE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : TOUCH_RST_PC13_Pin */
  GPIO_InitStruct.Pin = TOUCH_RST_PC13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TOUCH_RST_PC13_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TOUCH_EN_Pin */
  GPIO_InitStruct.Pin = TOUCH_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TOUCH_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CAN1_S0_Pin CAN2_S0_Pin */
  GPIO_InitStruct.Pin = CAN1_S0_Pin|CAN2_S0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : HALL_OUT_Pin BTN_2_PC6_Pin BTN_1_PC7_Pin BTN_4_PE5_Pin */
  GPIO_InitStruct.Pin = HALL_OUT_Pin|BTN_2_PC6_Pin|BTN_1_PC7_Pin|BTN_4_PE5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_CS_PIN_Pin */
  GPIO_InitStruct.Pin = SD_CS_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_CS_PIN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : IN_S0_Pin IN_S1_Pin IN_S2_Pin PUD_E_Pin */
  GPIO_InitStruct.Pin = IN_S0_Pin|IN_S1_Pin|IN_S2_Pin|PUD_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : IN_E_Pin OUT_S0_Pin PUD_S2_Pin PUD_S1_Pin
                           PUD_S0_Pin */
  GPIO_InitStruct.Pin = IN_E_Pin|OUT_S0_Pin|PUD_S2_Pin|PUD_S1_Pin
                          |PUD_S0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : FLASH_CS_Pin */
  GPIO_InitStruct.Pin = FLASH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(FLASH_CS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */



float mapFloat(float x, float in_min, float in_max, float out_min,
		float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long mapInt(float x, float in_min, float in_max, int out_min, int out_max) {
	return (int) ((x - in_min) * (out_max - out_min) / (in_max - in_min)
			+ out_min);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == INT_PIN)
  {
	  TOUCH_Set();
  }
}



char* build_status_json(void) {
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;

    cJSON_AddStringToObject(root, "type", "update");

    cJSON *payload = cJSON_CreateObject();
    if (!payload) {
        cJSON_Delete(root);
        return NULL;
    }

    // Добавляем поля из структуры, соответствующие вашему примеру JSON
    cJSON_AddNumberToObject(payload, "rpm", Current_Status.RPM);
    cJSON_AddNumberToObject(payload, "map", Current_Status.MAP);
    cJSON_AddNumberToObject(payload, "tps", Current_Status.TPS1);
    cJSON_AddNumberToObject(payload, "afr", Current_Status.AFR);
    cJSON_AddNumberToObject(payload, "IgnTiming", Current_Status.IgnitionTiming);
    cJSON_AddNumberToObject(payload, "motorhours", Current_Status.MOTOHOURS);
    cJSON_AddNumberToObject(payload, "pps", Current_Status.PPS);
    cJSON_AddNumberToObject(payload, "speed", Current_Status.GPS_SPEED);
    cJSON_AddNumberToObject(payload, "clt", Current_Status.CoolantTemp);
    cJSON_AddNumberToObject(payload, "iat", Current_Status.IntakeTemp);
    cJSON_AddNumberToObject(payload, "oilTemp", Current_Status.OilTemperature);
    cJSON_AddNumberToObject(payload, "oilPress", Current_Status.OilPress);
    cJSON_AddNumberToObject(payload, "oilLevel", Current_Status.OilLevel); // В структуре FuelFlow - можно заменить на FuelPress если есть
    cJSON_AddNumberToObject(payload, "fuelLevelraw", Current_Status.FuelLevelRaw);
    cJSON_AddNumberToObject(payload, "knockCt", Current_Status.KnockCt);
    cJSON_AddNumberToObject(payload, "fueluseds", Current_Status.FUELUSEDs);
//    cJSON_AddNumberToObject(payload, "statusFlags", Current_Status.ENGINE_PROTECTION);
//    cJSON_AddNumberToObject(payload, "errorCode", Current_Status.LastError);
    cJSON_AddNumberToObject(payload, "battVolt", Current_Status.BattVolt);
 //   cJSON_AddNumberToObject(payload, "knockLevel", Current_Status.KnockCt);

    cJSON_AddItemToObject(root, "payload", payload);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str; // Освободить после использования через free()
}






void InitFlags(void) {
    flags_union.flags.CAN_ABIT = 0;
    flags_union.flags.CAN_RUSEFI = 1;
}

// Глобальная функция обработки сообщений CAN
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
        // Ошибка при получении сообщения CAN
        return;
    }

    // Если CAN не включен, выходим (если нужно)
    // if (!Current_Status.CAN_ENABLED) return;

    // --- Переключение между протоколами ---
    if (flags_union.flags.CAN_RUSEFI && !flags_union.flags.CAN_ABIT) {
        // --- rusefi протокол ---
        switch (RxHeader.StdId) {
            case 3221225472: Handle_BO_3221225472(RxData); break;
            case 512: Handle_BO_512(RxData); break;
            case 513: Handle_BO_513(RxData); break;
            case 514: Handle_BO_514(RxData); break;
            case 515: Handle_BO_515(RxData); break;
            case 516: Handle_BO_516(RxData); break;
            case 517: Handle_BO_517(RxData); break;
            case 518: Handle_BO_518(RxData); break;
            case 519: Handle_BO_519(RxData); break;
            case 520: Handle_BO_520(RxData); break;
            case 521: Handle_BO_521(RxData); break;
            default:
                // Обработка неизвестного ID CAN для rusefi
                break;
        }
    } else if (!flags_union.flags.CAN_RUSEFI && flags_union.flags.CAN_ABIT) {
        // --- ABIT протокол ---
        switch (RxHeader.StdId) {
            case 0x281: Handle_BO_0x281(RxData); break;
            case 0x282: Handle_BO_0x282(RxData); break;
            case 0x283: Handle_BO_0x283(RxData); break;
            case 0x284: Handle_BO_0x284(RxData); break;
            case 0x381: Handle_BO_0x381(RxData); break;
            case 0x382: Handle_BO_0x382(RxData); break;
            case 0x383: Handle_BO_0x383(RxData); break;
            case 0x481: Handle_BO_0x481(RxData); break;
            case 0x482: Handle_BO_0x482(RxData); break;
            case 0x581: Handle_BO_0x581(RxData); break;
            case 0x582: Handle_BO_0x582(RxData); break;
            case 0x583: Handle_BO_0x583(RxData); break;
            case 0x584: Handle_BO_0x584(RxData); break;
            case 0x781: Handle_BO_0x781(RxData); break;
//            case 0x211: Handle_BO_0x211(RxData); break;
//            case 0x212: Handle_BO_0x212(RxData); break;
//            case 0x511: Handle_BO_0x511(RxData); break;
//            case 0x512: Handle_BO_0x512(RxData); break;
//            case 0x385: Handle_BO_0x385(RxData); break;
//            case 0x784: Handle_BO_0x784(RxData); break;
            default:
                // Обработка неизвестного ID CAN для ABIT
                break;
        }
    } else {
        // Некорректная конфигурация протоколов (оба флага или ни одного)
        // Можно добавить логирование или аварийную индикацию
    }

    // --- Обработка ошибок ---

 /*   if (Current_Status.LastError != 0) {
        Current_Status.IND_DTC = true;
        Current_Status.Error_Mes = Error_code(Current_Status.LastError);
        Current_Status.container1 = true;
    } else {
        Current_Status.container1 = false;
        Current_Status.IND_DTC = false;
    }*/
}







 // оброботчик прерываний прием передача USART3

void json_rx_callback(const char* json_str)
{
    if (!json_str) return;
    // Кладём строку в очередь для задачи
    osMessageQueuePut(jsonQueueHandle, json_str, 0, 0);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    USART3_JSON_RxEventCallback(huart, Size);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
     USART3_JSON_TxCpltCallback(huart);
}


void app_init_usart3(void)
{
    USART3_JSON_Init(&huart3, 115200, json_rx_callback);
}

void initAll(void) {

	Current_Status.LCD_BRIGHTNESS = LCD_DEFAULT_BRIGHTNESS;
	Current_Status.LCD_BRIGHTNESS_CHANGED = 1;
	htim13.Instance->CCR1 = Current_Status.LCD_BRIGHTNESS;

	Current_Status.CAN_ENABLED = 0;

	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);

//	LoadFlagsFromEEPROM();

}
//  сохронение флагов в епром
void SaveFlagsToEEPROM(void) {
    flags_union.flags = device_flags; // Копируем структуру в union
    EEPROM_WriteFlags(flags_union.raw);

}

// чтение флагов в епром
	bool LoadFlagsFromEEPROM(void) {
	    if (EEPROM_ReadDual(eeprom_cfg.flags_main_addr, eeprom_cfg.flags_backup_addr, &flags_union.raw, sizeof(flags_union.raw))) {
	        device_flags = flags_union.flags; // Копируем обратно в структуру
	        return true;
	    }
	    return false;
	}

// Среднее скользящее

typedef struct {
    float *values;  // Указатель на массив значений
    int index;
    int count;
    int windowSize; // Размер окна
} MovingAverage;

// Функция инициализации скользящего среднего
void initMovingAverage(MovingAverage *ma, int windowSize) {
    ma->windowSize = windowSize;
    ma->index = 0;
    ma->count = 0;
    ma->values = (float *)malloc(windowSize * sizeof(float)); // Динамическое выделение памяти

    for (int i = 0; i < windowSize; i++) {
        ma->values[i] = 0.0f;
    }
}

// Функция освобождения ресурсов
void freeMovingAverage(MovingAverage *ma) {
    free(ma->values); // Освобождение выделенной памяти
}

// Функция обновления скользящего среднего
float updateMovingAverage(MovingAverage *ma, float newValue) {
    ma->values[ma->index] = newValue;
    ma->index = (ma->index + 1) % ma->windowSize;

    if (ma->count < ma->windowSize) {
        ma->count++;
    }

    float sum = 0.0f;
    for (int i = 0; i < ma->count; i++) {
        sum += ma->values[i];
    }

    return sum / ma->count; // Возврат SMA
}

// округление до кратного

int roundToNearest(int value, int multiple) {
    // Находим остаток от деления на кратное значение
    int remainder = value % multiple;

    // Если остаток меньше половины от кратного значения, округляем вниз
    // Иначе округляем вверх
    if (remainder < multiple / 2) {
        return value - remainder; // Округляем вниз
    } else {
        return value + (multiple - remainder); // Округляем вверх
    }
}



/* USER CODE END 4 */

/* USER CODE BEGIN Header_Start_START_Task */
/**
 * @brief  Function implementing the START_Task thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_START_Task */
void Start_START_Task(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 5 */
	osDelay(250);


	Current_Status.RPM = 0;
	Current_Status.CAN_ENABLED = 1;
	Current_Status.FuelUsed = 0;

	fuel_level_load_table();
	oil_level_load_table();

	for (;;) {

		float raw_fuel = Current_Status.FuelLevelRaw;
	    Current_Status.FuelLevel = fuel_level_get_liters(raw_fuel);
	    Current_Status.FuelLevelGui =  (100/fuelLevelMax)*Current_Status.FuelLevel;
       osDelay(10);
	}

  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_TouchGFX_Task */
/**
 * @brief Function implementing the TouchGFXTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_TouchGFX_Task */
__weak void TouchGFX_Task(void *argument)
{
  /* USER CODE BEGIN TouchGFX_Task */
//	MX_TouchGFX_Process();

	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END TouchGFX_Task */
}

/* USER CODE BEGIN Header_Start_BRIGHTNESS_Task */
/**
* @brief Function implementing the BRIGHTNESS_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_BRIGHTNESS_Task */
void Start_BRIGHTNESS_Task(void *argument)
{
  /* USER CODE BEGIN Start_BRIGHTNESS_Task */
  /* Infinite loop */
  for(;;)
  {
	  if(Current_Status.LCD_BRIGHTNESS > 1000){
	  	 Current_Status.LCD_BRIGHTNESS = 300;
	     }
	  htim13.Instance->CCR1 = Current_Status.LCD_BRIGHTNESS;
	  Current_Status.LCD_BRIGHTNESS_CHANGED = 0;
    osDelay(1);
  }
  /* USER CODE END Start_BRIGHTNESS_Task */
}

/* USER CODE BEGIN Header_Start_nmea */
/**
* @brief Function implementing the nmea thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_nmea */
void Start_nmea(void *argument)
{
  /* USER CODE BEGIN Start_nmea */
  /* Infinite loop */
	nmea_init(&gps, USART1, 1024);
	  for(;;)
	  {
	    nmea_loop(&gps);
	    osDelay(100);
	  }
  /* USER CODE END Start_nmea */
}

/* USER CODE BEGIN Header_Start_OIL */
/**
* @brief Function implementing the Ind_OIL thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_OIL */
void Start_OIL(void *argument)
{
  /* USER CODE BEGIN Start_OIL */
	ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_11;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  /* Infinite loop */
  for(;;)
  {

	  		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
	  			Error_Handler();
	  		}

	  		uint32_t ADCValue = 0;
	  		HAL_ADC_Start(&hadc1);
	  		HAL_ADC_PollForConversion(&hadc1, 1000);
	  		ADCValue = HAL_ADC_GetValue(&hadc1);
	  		HAL_ADC_Stop(&hadc1);
	  		Current_Status.IND_OIL = (ADCValue * 749) * (3.3 / 4096.0);

	  		Current_Status.IND_OIL = Current_Status.IND_OIL < 100 ? true : false;

	  //		Current_Status.IND_DTC = (Current_Status.WarningCounter != 0) ? true : false;
    osDelay(500);
  }
  /* USER CODE END Start_OIL */
}

/* USER CODE BEGIN Header_Start_Ind_FUEL */
/**
* @brief Function implementing the Ind_FUEL thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Ind_FUEL */
void Start_Ind_FUEL(void *argument)
{
  /* USER CODE BEGIN Start_Ind_FUEL */
  /* Infinite loop */
  for(;;)
  {
	  uint8_t fuelLevelPercentage = Current_Status.FuelLevel ;

	  if (fuelLevelPercentage < 5) {
	       Current_Status.IND_FUEL = true;
	       osDelay(300);
	       Current_Status.IND_FUEL = false;
	       osDelay(300);
	     } else if (fuelLevelPercentage < 8) {
	       Current_Status.IND_FUEL = true;
	     } else {
	       Current_Status.IND_FUEL = false;
	     }
    osDelay(10);
  }
  /* USER CODE END Start_Ind_FUEL */
}

/* USER CODE BEGIN Header_Start_IndBATT */
/**
* @brief Function implementing the Ind_BATT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_IndBATT */
void Start_IndBATT(void *argument)
{
  /* USER CODE BEGIN Start_IndBATT */
  /* Infinite loop */
  for(;;)
  {

	  float batteryLevel = Current_Status.BattVolt ;

	     if (batteryLevel < 11) {
	        Current_Status.IND_BATT = true;
	        osDelay(300);
	        Current_Status.IND_BATT = false;
	        osDelay(300);
	      } else {
	        Current_Status.IND_BATT = false;
	      }
    osDelay(10);
  }
  /* USER CODE END Start_IndBATT */
}

/* USER CODE BEGIN Header_Start_Ind_ECT */
/**
* @brief Function implementing the Ind_ECT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Ind_ECT */
void Start_Ind_ECT(void *argument)
{
  /* USER CODE BEGIN Start_Ind_ECT */
  /* Infinite loop */
  for(;;)
  {
	  float ect_temp = Current_Status.CoolantTemp / 10.0f; // Вычисляем один раз

	  	  // Индикатор синего цвета
	  	  Current_Status.IND_ECT_BLUE = (ect_temp < 50);

	  	  // Индикатор перегрева (мигание)
	  	  if (ect_temp > 115) {
	  		  Current_Status.IND_ECT = true;
	  		  osDelay(300);
	  		  Current_Status.IND_ECT = false;
	  		  osDelay(300);
	  	  } else {
	  		  Current_Status.IND_ECT = false;
	  	  }
    osDelay(10);
  }
  /* USER CODE END Start_Ind_ECT */
}

/* USER CODE BEGIN Header_Start_other */
/**
* @brief Function implementing the other thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_other */
void Start_other(void *argument)
{
  /* USER CODE BEGIN Start_other */
  /* Infinite loop */
	float speed_kph = 0;

	  for(;;)
	  {
	    if (nmea_available(&gps))
	    {
	      nmea_gnss_speed_kph(&gps,&speed_kph);
	      Current_Status.GPS_SPEED = speed_kph;
	      nmea_available_reset(&gps);
	   }

	    osDelay(100);
	  }
  /* USER CODE END Start_other */
}

/* USER CODE BEGIN Header_Start_RMP_10 */
/**
* @brief Function implementing the RPM_10 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_RMP_10 */
void Start_RMP_10(void *argument)
{
  /* USER CODE BEGIN Start_RMP_10 */
	    int windowSize = *((int *)argument);
	    MovingAverage rpmMA;
	    initMovingAverage(&rpmMA, windowSize);
	    /* Infinite loop */
	    for(;;)
	    {
	        float rpmValue = Current_Status.RPM; // Получение текущего значения RPM
	        float avgRPM = updateMovingAverage(&rpmMA, rpmValue);

	        Current_Status.RPMs = roundToNearest(avgRPM, 25); //   ближайшего значения кратного 25
	        osDelay(50);
	    }
	    freeMovingAverage(&rpmMA); // Освобождение памяти при завершении задачи (если вы используете динамическое выделение)

  /* USER CODE END Start_RMP_10 */
}

/* USER CODE BEGIN Header_Start_FUELUSED_11 */
/**
* @brief Function implementing the FUELUSED_11 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_FUELUSED_11 */
void Start_FUELUSED_11(void *argument)
{
  /* USER CODE BEGIN Start_FUELUSED_11 */
	int *windows = (int *)argument;
	int fuelWindow = windows[0];
	int remainsWindow = windows[1];

	MovingAverage fuelUserMA, remainsMA;
	initMovingAverage(&fuelUserMA, fuelWindow);    // Для FuelUsed
	initMovingAverage(&remainsMA, remainsWindow);  // Для RemainsKm

  /* Infinite loop */
  for(;;)
  {
	  // Получаем актуальные значения!
	          float fuel_left_liters = Current_Status.FuelLevel;   // Остаток топлива, литров
	          float avg_consumption_lph = Current_Status.FUELUSEDs; // Средний расход, л/ч
	          float speed_kmh = Current_Status.GPS_SPEED;          // Скорость, км/ч
//	          float remains_km1 = 0 ;
	          // Обновление FuelUsed
	                  float fuelValue = Current_Status.FuelFlow;
	                  Current_Status.FUELUSEDs = updateMovingAverage(&fuelUserMA, fuelValue) + 0.0001f;

	                  // --- Защита от деления на ноль и некорректных значений ---
	                  float raw_remains = 0.0f;
	                  if (avg_consumption_lph > 0.01f && speed_kmh > 0.1f && fuel_left_liters > 0.01f) {
	                      raw_remains = (fuel_left_liters * speed_kmh) / avg_consumption_lph;
	                  } else {
	                      raw_remains = 0.0f; // если расход или скорость слишком малы, запас хода не считаем
	                  }
	                  Current_Status.remains_km = updateMovingAverage(&remainsMA, raw_remains);

	                  // Для отображения целого значения (например, на дисплее)
//	                  Current_Status.remains_km_int = (int)(Current_Status.remains_km + 0.5f);


	              }

	              freeMovingAverage(&fuelUserMA);
	              freeMovingAverage(&remainsMA);


	              osDelay(100);
  /* USER CODE END Start_FUELUSED_11 */
}

/* USER CODE BEGIN Header_Start_UART_Task */
/**
* @brief Function implementing the UART_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_UART_Task */
void Start_UART_Task(void *argument)
{
  /* USER CODE BEGIN Start_UART_Task */

  /* Infinite loop */
  for(;;){
	  // Формируем JSON-строку (build_status_json должен возвращать malloc-строку!)
/*	  char* json_str = build_data_json(1, 16, fuelLevelTable);
	  if (json_str)
	  {
	      // Ждём, пока UART не освободится (максимум 100 попыток по 1 мс)
	      uint32_t try_cnt = 0;
	      while (USART3_JSON_IsTxBusy())
	      {
	          osDelay(1);
	          if (++try_cnt > 100)
	          {
	              // UART завис? Освобождаем память и выходим из цикла
	              free(json_str);
	              json_str = NULL;
	              break;
	          }
	      }
	      if (json_str)
	      {
	          if (USART3_JSON_Send(json_str) == HAL_OK)
	          {
	              // Можно добавить логирование успеха
	          }
	          else
	          {
	              // Ошибка отправки, можно залогировать
	          }
	          free(json_str);
	      }
	  }*/

	        osDelay(200); // Пауза между отправками (200 мс)
	    }
  /* USER CODE END Start_UART_Task */
}

/* USER CODE BEGIN Header_Start_MotohoursTask */
/**
* @brief Function implementing the MotohoursTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_MotohoursTask */
void Start_MotohoursTask(void *argument)
{
  /* USER CODE BEGIN Start_MotohoursTask */
	 osDelay(100);
	    if (!EEPROM_ReadMotohours(&Current_Status.MOTOHOURS)) {
	        Current_Status.MOTOHOURS = 0;
	    }
	    uint32_t last_saved = Current_Status.MOTOHOURS;
	    const uint32_t SAVE_INTERVAL = 240; // сохранять раз в минуту
  /* Infinite loop */
  for(;;)
  {
	  osDelay(1000);
	  if (Current_Status.RPM > 100)
	  {// 1 секунда
	  Current_Status.MOTOHOURS++;
	  // Сохраняем только если прошло SAVE_INTERVAL секунд
	   if ((Current_Status.MOTOHOURS - last_saved) >= SAVE_INTERVAL) {
	       if (EEPROM_WriteMotohours(Current_Status.MOTOHOURS) == HAL_OK) {
	           eeprom_wait_ready(); // обязательно дождаться окончания записи!
	           }   last_saved = Current_Status.MOTOHOURS;
	      }
	}
  }
  /* USER CODE END Start_MotohoursTask */
}

/* USER CODE BEGIN Header_Start_R_Buttan_Task14 */
/**
* @brief Function implementing the Button_R_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_R_Buttan_Task14 */
void Start_R_Buttan_Task14(void *argument)
{
  /* USER CODE BEGIN Start_R_Buttan_Task14 */
  for(;;)
  {
    // Более безопасная проверка с использованием else-if
	  if(Current_Status.RPM < (RPM_THRESHOLD - RPM_HYSTERESIS))
    {
        Current_Status.container4 = false;
        Current_Status.container6 = true;
    }
    else if(Current_Status.RPM > (RPM_THRESHOLD + RPM_HYSTERESIS))// RPM >= 500
    {
    	osDelay(5000);
        Current_Status.container4 = true;
        Current_Status.container6 = false;
    }


    osDelay(1);
  }
  /* USER CODE END Start_R_Buttan_Task14 */
}

/* USER CODE BEGIN Header_Start_Buttan_L_Task */
/**
* @brief Function implementing the Button_L_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Buttan_L_Task */
void Start_Buttan_L_Task(void *argument)
{
  /* USER CODE BEGIN Start_Buttan_L_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_Buttan_L_Task */
}

/* USER CODE BEGIN Header_Start_JSON_Parse_Task */
/**
* @brief Function implementing the JSON_Parse_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_JSON_Parse_Task */
void Start_JSON_Parse_Task(void *argument)
{
  /* USER CODE BEGIN Start_JSON_Parse_Task */



   JsonMessage msg;
  /* Infinite loop */
  for(;;)
  {

  	  /// Получаем строку JSON из очереди (блокирующий вызов)
      if (osMessageQueueGet(jsonQueueHandle, json_buf, NULL, osWaitForever) == osOK)
      {
          memset(&msg, 0, sizeof(msg));
          if (parse_and_dispatch_json(json_buf, &msg))
          {
              switch (msg.type)
              {
                  case JSON_TYPE_COMMAND:
                      handle_command(&msg.payload.command);
                      break;
                  case JSON_TYPE_UPDATE:
                      // handle_status_update(&msg.payload.status);
                      break;
                  case JSON_TYPE_FLAGS:
                      // handle_flags_update(&msg.payload.flags);
                      break;
                  case JSON_TYPE_DATA:
                      handle_data(&msg.payload.data);
                      break;
                  case JSON_TYPE_REQUEST:
                      // handle_request(&msg.payload.request);
                      break;
                  default:
                      // Логировать неизвестный тип
                      break;
              }
          }
          else
          {
              // Логировать ошибку парсинга
          }
     }
osDelay(1);
  }
  /* USER CODE END Start_JSON_Parse_Task */
}

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
  if (htim->Instance == TIM6)
  {
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
