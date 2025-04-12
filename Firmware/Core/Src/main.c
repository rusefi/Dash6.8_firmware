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
#include "WS2812/WS2812.hpp"
#include "nmea.h"
#include "TargetTouch.h"
//#include "spi_flash.h"
//#include "mcu_flash.h"


//Setup
#include "Setup/Dash/setupDash.h"
#include "Setup/Field/setupField.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include <math.h>

#define EXTRACT_U16(data, start_bit) ((uint16_t)(((data)[(start_bit) / 8] >> ((start_bit) % 8)) | ((data)[((start_bit) / 8) + 1] << (8 - ((start_bit) % 8)))))

nmea_t gps;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
int rpmWindowSize = 10;     //  размер окна для RPM
int fuelUserWindowSize = 5; //  размер окна для FUELUSER
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
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Ind_FUEL */
osThreadId_t Ind_FUELHandle;
const osThreadAttr_t Ind_FUEL_attributes = {
  .name = "Ind_FUEL",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Ind_BATT */
osThreadId_t Ind_BATTHandle;
const osThreadAttr_t Ind_BATT_attributes = {
  .name = "Ind_BATT",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Ind_ECT */
osThreadId_t Ind_ECTHandle;
const osThreadAttr_t Ind_ECT_attributes = {
  .name = "Ind_ECT",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for other */
osThreadId_t otherHandle;
const osThreadAttr_t other_attributes = {
  .name = "other",
  .stack_size = 64 * 4,
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
/* USER CODE BEGIN PV */
FMC_SDRAM_CommandTypeDef command;

Statuses Current_Status;

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];

uint8_t uartTransmitBufferSize;
uint8_t uartTransmitBuffer[128];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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

/* USER CODE BEGIN PFP */

void initAll(void);
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
  MX_USART3_UART_Init();
  MX_I2C3_Init();
  MX_TouchGFX_Init();
  /* Call PreOsInit function */
  MX_TouchGFX_PreOSInit();
  /* USER CODE BEGIN 2 */

  initAll();



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
  RPM_10Handle = osThreadNew(Start_RMP_10, &rpmWindowSize, &RPM_10_attributes);

  /* creation of FUELUSED_11 */
  FUELUSED_11Handle = osThreadNew(Start_FUELUSED_11, &fuelUserWindowSize, &FUELUSED_11_attributes);

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
  hltdc.Init.HorizontalSync = 4;
  hltdc.Init.VerticalSync = 29;
  hltdc.Init.AccumulatedHBP = 9;
  hltdc.Init.AccumulatedVBP = 69;
  hltdc.Init.AccumulatedActiveW = 489;
  hltdc.Init.AccumulatedActiveH = 1349;
  hltdc.Init.TotalWidth = 494;
  hltdc.Init.TotalHeigh = 1369;
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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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



// Глобальная функция обработки сообщений CAN
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
    // Ошибка при получении сообщения CAN. Добавьте обработку ошибок.
    return;
  }

  if (!Current_Status.CAN_ENABLED) {
    return; // CAN не включен, выходим
  }

  HAL_GPIO_TogglePin(LED_CAN1_GPIO_Port, LED_CAN1_Pin);

  // Маршрутизация сообщений на основе ID
  switch (RxHeader.StdId) {
    case 3221225472:
      Handle_BO_3221225472(RxData);
      break;
    case 512:
      Handle_BO_512(RxData);
      break;
    case 513:
      Handle_BO_513(RxData);
      break;
    case 514:
      Handle_BO_514(RxData);
      break;
    case 515:
      Handle_BO_515(RxData);
      break;
    case 516:
      Handle_BO_516(RxData);
      break;
    case 517:
      Handle_BO_517(RxData);
      break;
    case 518:
      Handle_BO_518(RxData);
      break;
    case 519:
      Handle_BO_519(RxData);
      break;
    case 520:
      Handle_BO_520(RxData);
      break;
    case 521:
         Handle_BO_521(RxData);
         break;
    default:
      // Обработка неизвестного ID CAN
      break;
  }
}

// Обработчики сообщений CAN

void Handle_BO_3221225472(uint8_t *data) {
  // SG_ AFR : 7|16@0+ (0.001,0) [0|0] "AFR"
  uint16_t AFR_raw = EXTRACT_U16(data, 7);
  Current_Status.AFR = AFR_raw * 0.001;

  // SG_ VVTPos : 24|16@1- (0.02,0) [0|0] "deg"
  uint16_t VVTPos_raw = EXTRACT_U16(data, 24);
  Current_Status.VVTPos = (int16_t)VVTPos_raw * 0.02;

  // SG_ NewSignal_0010 : 24|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0010 = (data[3] >> 0) & 0xFF;

  // SG_ NewSignal_0009 : 16|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0009 = (data[2] >> 0) & 0xFF;

    // SG_ NewSignal_0008 : 8|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0008 = (data[1] >> 0) & 0xFF;

    // SG_ NewSignal_0015 : 8|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0015 = (data[1] >> 0) & 0xFF;

    // SG_ NewSignal_0016 : 16|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0016 = (data[2] >> 0) & 0xFF;

   // SG_ NewSignal_0024 : 0|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0024 = (data[0] >> 0) & 0xFF;
}

void Handle_BO_512(uint8_t *data) {
  // SG_ WarningCounter : 0|16@1+ (1,0) [0|0] ""
  Current_Status.WarningCounter = EXTRACT_U16(data, 0);

  // SG_ LastError : 16|16@1+ (1,0) [0|0] ""
  Current_Status.LastError = EXTRACT_U16(data, 16);

  // SG_ RevLimAct : 32|1@1+ (1,0) [0|0] ""
  Current_Status.RevLimAct = (data[4] >> 0) & 0x01;

  // SG_ MainRelayAct : 33|1@1+ (1,0) [0|0] ""
  Current_Status.MainRelayAct = (data[4] >> 1) & 0x01;

  // SG_ FuelPumpAct : 34|1@1+ (1,0) [0|0] ""
  Current_Status.FuelPumpAct = (data[4] >> 2) & 0x01;

  // SG_ CELAct : 35|1@1+ (1,0) [0|0] ""
  Current_Status.CELAct = (data[4] >> 3) & 0x01;

  // SG_ EGOHeatAct : 36|1@1+ (1,0) [0|0] ""
  Current_Status.EGOHeatAct = (data[4] >> 4) & 0x01;

  // SG_ LambdaProtectAct : 37|1@1+ (1,0) [0|0] ""
  Current_Status.LambdaProtectAct = (data[4] >> 5) & 0x01;

  // SG_ Fan : 38|1@1+ (1,0) [0|0] ""
  Current_Status.Fan = (data[4] >> 6) & 0x01;

  // SG_ Fan2 : 39|1@1+ (1,0) [0|0] ""
  Current_Status.Fan2 = (data[4] >> 7) & 0x01;

  // SG_ CurrentGear : 40|8@1+ (1,0) [0|0] ""
  Current_Status.CurrentGear = (data[5] >> 0) & 0xFF;

  // SG_ DistanceTraveled : 48|16@1+ (0.1,0) [0|6553.5] "km"
  uint16_t DistanceTraveled_raw = EXTRACT_U16(data, 48);
  Current_Status.DistanceTraveled = DistanceTraveled_raw * 0.1;
}

void Handle_BO_513(uint8_t *data) {
  // SG_ RPM : 0|16@1+ (1,0) [0|0] "RPM"
  Current_Status.RPM = EXTRACT_U16(data, 0);

  // SG_ IgnitionTiming : 16|16@1- (0.02,0) [0|0] "deg"
  uint16_t IgnitionTiming_raw = EXTRACT_U16(data, 16);
  Current_Status.IgnitionTiming = (int16_t)IgnitionTiming_raw * 0.02;

  // SG_ InjDuty : 32|8@1+ (0.5,0) [0|100] "%"
  Current_Status.InjDuty = ((data[4] >> 0) & 0xFF) * 0.5;

  // SG_ IgnDuty : 40|8@1+ (0.5,0) [0|100] "%"
  Current_Status.IgnDuty = ((data[5] >> 0) & 0xFF) * 0.5;

  // SG_ VehicleSpeed : 48|8@1+ (1,0) [0|255] "kph"
  Current_Status.VehicleSpeed = (data[6] >> 0) & 0xFF;

  // SG_ FlexPct : 56|8@1+ (1,0) [0|100] "%"
  Current_Status.FlexPct = (data[7] >> 0) & 0xFF;
}

void Handle_BO_514(uint8_t *data) {
  // SG_ PPS : 0|16@1- (0.01,0) [0|100] "%"
  uint16_t PPS_raw = EXTRACT_U16(data, 0);
  Current_Status.PPS = (int16_t)PPS_raw * 0.01;

  // SG_ TPS1 : 16|16@1- (0.01,0) [0|100] "%"
  uint16_t TPS1_raw = EXTRACT_U16(data, 16);
  Current_Status.TPS1 = (int16_t)TPS1_raw * 0.01;

  // SG_ TPS2 : 32|16@1- (0.01,0) [0|100] "%"
  uint16_t TPS2_raw = EXTRACT_U16(data, 32);
  Current_Status.TPS2 = (int16_t)TPS2_raw * 0.01;

  // SG_ Wastegate : 48|16@1- (0.01,0) [0|100] "%"
  uint16_t Wastegate_raw = EXTRACT_U16(data, 48);
  Current_Status.Wastegate = (int16_t)Wastegate_raw * 0.01;
}

void Handle_BO_515(uint8_t *data) {
  // SG_ MAP : 0|16@1+ (0.03333333,0) [0|0] "kPa"
  uint16_t MAP_raw = EXTRACT_U16(data, 0);
  Current_Status.MAP = MAP_raw * 0.03333333;

  // SG_ CoolantTemp : 16|8@1+ (1,-40) [-40|200] "deg C"
  Current_Status.CoolantTemp = ((data[2] >> 0) & 0xFF) * 1 - 40;

  // SG_ IntakeTemp : 24|8@1+ (1,-40) [-40|200] "deg C"
  Current_Status.IntakeTemp = ((data[3] >> 0) & 0xFF) * 1 - 40;

  // SG_ AUX1Temp : 32|8@1+ (1,-40) [-40|200] "deg C"
  Current_Status.AUX1Temp = ((data[4] >> 0) & 0xFF) * 1 - 40;

  // SG_ AUX2Temp : 40|8@1+ (1,-40) [-40|200] "deg C"
  Current_Status.AUX2Temp = ((data[5] >> 0) & 0xFF) * 1 - 40;

  // SG_ MCUTemp : 48|8@1+ (1,-40) [-40|100] "deg C"
  Current_Status.MCUTemp = ((data[6] >> 0) & 0xFF) * 1 - 40;

  // SG_ FuelLevel : 56|8@1+ (0.5,0) [0|0] "%"
  Current_Status.FuelLevel = ((data[7] >> 0) & 0xFF) * 0.5;
}

void Handle_BO_516(uint8_t *data) {
  // SG_ OilPress : 16|16@1+ (0.03333333,0) [0|0] "kPa"
  uint16_t OilPress_raw = EXTRACT_U16(data, 16);
  Current_Status.OilPress = OilPress_raw * 0.03333333;

  // SG_ OilTemperature : 32|8@1+ (1,-40) [-40|215] "deg C"
  Current_Status.OilTemperature = ((data[4] >> 0) & 0xFF) * 1 - 40;

  // SG_ FuelTemperature : 40|8@1+ (1,-40) [-40|215] "deg C"
  Current_Status.FuelTemperature = ((data[5] >> 0) & 0xFF) * 1 - 40;

  // SG_ BattVolt : 48|16@1+ (0.001,0) [0|25] "mV"
  uint16_t BattVolt_raw = EXTRACT_U16(data, 48);
  Current_Status.BattVolt = BattVolt_raw * 0.001;
}

void Handle_BO_517(uint8_t *data) {
  // SG_ CylAM : 0|16@1+ (1,0) [0|0] "mg"
  Current_Status.CylAM = EXTRACT_U16(data, 0);

  // SG_ EstMAF : 16|16@1+ (0.01,0) [0|0] "kg/h"
  Current_Status.EstMAF = EXTRACT_U16(data, 16) * 0.01;

  // SG_ InjPW : 32|16@1+ (0.003333333,0) [0|0] "ms"
  Current_Status.InjPW = EXTRACT_U16(data, 32) * 0.003333333;

  // SG_ KnockCt : 48|16@1+ (1,0) [0|0] "count"
  Current_Status.KnockCt = EXTRACT_U16(data, 48);
}

void Handle_BO_518(uint8_t *data) {
  // SG_ FuelUsed : 0|16@1+ (1,0) [0|0] "g"
  Current_Status.FuelUsed = EXTRACT_U16(data, 0);

  // SG_ FuelFlow : 16|16@1+ (0.005,0) [0|327] "g/s"
  Current_Status.FuelFlow = EXTRACT_U16(data, 16) * 0.005;

  // SG_ FuelTrim1 : 32|16@1+ (0.01,0) [-50|50] "%"
  Current_Status.FuelTrim1 = EXTRACT_U16(data, 32) * 0.01;

  // SG_ FuelTrim2 : 48|16@1+ (0.01,0) [-50|50] "%"
  Current_Status.FuelTrim2 = EXTRACT_U16(data, 48) * 0.01;
}

void Handle_BO_519(uint8_t *data) {
  // SG_ Lam1 : 0|16@1+ (0.0001,0) [0|2] "lambda"
  Current_Status.Lam1 = EXTRACT_U16(data, 0) * 0.0001;

  // SG_ Lam2 : 16|16@1+ (0.0001,0) [0|2] "lambda"
  Current_Status.Lam2 = EXTRACT_U16(data, 16) * 0.0001;

  // SG_ FpLow : 32|16@1+ (0.03333333,0) [0|0] "kPa"
  Current_Status.FpLow = EXTRACT_U16(data, 32) * 0.03333333;

  // SG_ FpHigh : 48|16@1+ (0.1,0) [0|0] "bar"
  Current_Status.FpHigh = EXTRACT_U16(data, 48) * 0.1;
}

void Handle_BO_520(uint8_t *data) {
    // SG_Cam1I : 0|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam1I = (int8_t)(data[0]);

    // SG_Cam1Itar : 8|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam1Itar = (int8_t)(data[1]);

    // SG_Cam1E : 16|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam1E = (int8_t)(data[2]);

    // SG_Cam1Etar : 24|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam1Etar = (int8_t)(data[3]);

    // SG_Cam2I : 32|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam2I = (int8_t)(data[4]);

    // SG_Cam2Itar : 40|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam2Itar = (int8_t)(data[5]);

    // SG_Cam2E : 48|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam2E = (int8_t)(data[6]);

    // SG_Cam2Etar : 56|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam2Etar = (int8_t)(data[7]);
}
    void Handle_BO_521(uint8_t *data) {
        // SG_Cam1I : 0|8@1- (1,0) [-100|100] "deg"
     Current_Status.WarningCounterv = (int16_t)(data[0]);

        // SG_Cam1Itar : 8|8@1- (1,0) [-100|100] "deg"
     Current_Status.LastErrorv = (int16_t)(data[1]);

        // SG_Cam1E : 16|8@1- (1,0) [-100|100] "deg"
     Current_Status.NextOBD2Error = (int16_t)(data[2]);
}



void initAll(void) {

	Current_Status.LCD_BRIGHTNESS = LCD_DEFAULT_BRIGHTNESS;
	Current_Status.LCD_BRIGHTNESS_CHANGED = 1;
	htim13.Instance->CCR1 = Current_Status.LCD_BRIGHTNESS;

	Current_Status.CAN_ENABLED = 0;

	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);



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

// округление до краного

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


	for (;;) {
       osDelay(1);
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

	  		Current_Status.IND_DTC = (Current_Status.WarningCounter != 0) ? true : false;
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
	     } else if (fuelLevelPercentage < 10) {
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
	 int windowSize = *((int *)argument); // Получаем размер окна из параметров
	 MovingAverage fuelUserMA;
	 initMovingAverage(&fuelUserMA, windowSize);
  /* Infinite loop */
  for(;;)
  {
	  float rpmValue = Current_Status.FuelFlow; // Получение текущего значения RPM
	  Current_Status.FUELUSEDs = updateMovingAverage(&fuelUserMA, rpmValue);

    osDelay(100);
  }
  freeMovingAverage(&fuelUserMA); // Освобождение памяти при завершении задачи (если вы используете динамическое выделение)
  /* USER CODE END Start_FUELUSED_11 */
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
