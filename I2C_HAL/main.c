
#include "main.h"

I2C_HandleTypeDef hi2c1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);


#define DEVICE_ADDR     0x53<<1
#define DEVID_R         0x00
#define POWER_CTL_R     0x2D
#define DATA_FORMAT_R   0x31

#define FOUR_G         0x01
#define RESET          0x00
#define SET_MEASURE_B  0x08   //1000=0x08
#define DATA_START_ADDR   0x32

#define FOUR_G_SCALE_FACT  0.0078;

void adx1_write (uint8_t reg, uint8_t value);
void adx1_init(void);
void adx1_read_value(uint8_t reg);

uint8_t data_rec[6];
uint8_t device_id;

int16_t x,y,z;
float xg,yg,zg;

int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();

	adx1_init();

  while (1)
  {
		adx1_read_value(DATA_START_ADDR);
		x = ((data_rec[1]<<8) | data_rec[0]);
		y = ((data_rec[3]<<8) | data_rec[2]);
		z = ((data_rec[5]<<8) | data_rec[4]);
		
		xg = x * FOUR_G_SCALE_FACT;
		yg = y* FOUR_G_SCALE_FACT;
		zg = z* FOUR_G_SCALE_FACT;
  }
}

void adx1_write (uint8_t reg, uint8_t value)
{
	uint8_t data[2];
	data[0]= reg;
	data[1]=value;
	
	HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDR, data, 2, 100);
}

void adx1_read_value(uint8_t reg)  //read the value 
{
	HAL_I2C_Mem_Read(&hi2c1,DEVICE_ADDR, reg, 1,  (uint8_t *)data_rec, 6, 100);
}

void adx1_read_address (uint8_t reg)  //read the value 
{
	HAL_I2C_Mem_Read(&hi2c1,DEVICE_ADDR, reg, 1, &device_id , 6, 100);
}
void adx1_init(void){
	//read address
	adx1_read_address(DEVID_R);
	//set the format register
	adx1_write(DATA_FORMAT_R, FOUR_G);
	//Reset all bits
	adx1_write(POWER_CTL_R, RESET);
	//Config PWR control
	adx1_write(POWER_CTL_R, SET_MEASURE_B);  
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
  __disable_irq();
  while (1)
  {
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

