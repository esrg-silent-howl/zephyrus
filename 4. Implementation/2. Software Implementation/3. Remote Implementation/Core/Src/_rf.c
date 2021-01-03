/**	
 * |----------------------------------------------------------------------
 * | Copyright (c) 2016 Tilen MAJERLE
 * |  
 * | Permission is hereby granted, free of charge, to any person
 * | obtaining a copy of this software and associated documentation
 * | files (the "Software"), to deal in the Software without restriction,
 * | including without limitation the rights to use, copy, modify, merge,
 * | publish, distribute, sublicense, and/or sell copies of the Software, 
 * | and to permit persons to whom the Software is furnished to do so, 
 * | subject to the following conditions:
 * | 
 * | The above copyright notice and this permission notice shall be
 * | included in all copies or substantial portions of the Software.
 * | 
 * | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * | OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * | AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * | HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * | WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * | FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * | OTHER DEALINGS IN THE SOFTWARE.
 * |----------------------------------------------------------------------
 */
#include "_rf.h"

/* NRF24L01+ registers*/
#define NRF24L01_REG_CONFIG			0x00	//Configuration Register
#define NRF24L01_REG_EN_AA			0x01	//Enable �Auto Acknowledgment� Function
#define NRF24L01_REG_EN_RXADDR		0x02	//Enabled RX Addresses
#define NRF24L01_REG_SETUP_AW		0x03	//Setup of Address Widths (common for all data pipes)
#define NRF24L01_REG_SETUP_RETR		0x04	//Setup of Automatic Retransmission
#define NRF24L01_REG_RF_CH			0x05	//RF Channel
#define NRF24L01_REG_RF_SETUP		0x06	//RF Setup Register	
#define NRF24L01_REG_STATUS			0x07	//Status Register
#define NRF24L01_REG_OBSERVE_TX		0x08	//Transmit observe registerf
#define NRF24L01_REG_RPD			0x09	
#define NRF24L01_REG_RX_ADDR_P0		0x0A	//Receive address data pipe 0. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P1		0x0B	//Receive address data pipe 1. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P2		0x0C	//Receive address data pipe 2. Only LSB
#define NRF24L01_REG_RX_ADDR_P3		0x0D	//Receive address data pipe 3. Only LSB
#define NRF24L01_REG_RX_ADDR_P4		0x0E	//Receive address data pipe 4. Only LSB
#define NRF24L01_REG_RX_ADDR_P5		0x0F	//Receive address data pipe 5. Only LSB
#define NRF24L01_REG_TX_ADDR		0x10	//Transmit address. Used for a PTX device only
#define NRF24L01_REG_RX_PW_P0		0x11	
#define NRF24L01_REG_RX_PW_P1		0x12	
#define NRF24L01_REG_RX_PW_P2		0x13	
#define NRF24L01_REG_RX_PW_P3		0x14	
#define NRF24L01_REG_RX_PW_P4		0x15	
#define NRF24L01_REG_RX_PW_P5		0x16	
#define NRF24L01_REG_FIFO_STATUS	0x17	//FIFO Status Register
#define NRF24L01_REG_DYNPD			0x1C	//Enable dynamic payload length
#define NRF24L01_REG_FEATURE		0x1D

/* Registers default values */
#define NRF24L01_REG_DEFAULT_VAL_CONFIG			0x08	
#define NRF24L01_REG_DEFAULT_VAL_EN_AA			0x3F	
#define NRF24L01_REG_DEFAULT_VAL_EN_RXADDR		0x03	
#define NRF24L01_REG_DEFAULT_VAL_SETUP_AW		0x03	
#define NRF24L01_REG_DEFAULT_VAL_SETUP_RETR		0x03	
#define NRF24L01_REG_DEFAULT_VAL_RF_CH			0x02	
#define NRF24L01_REG_DEFAULT_VAL_RF_SETUP		0x0E	
#define NRF24L01_REG_DEFAULT_VAL_STATUS			0x0E	
#define NRF24L01_REG_DEFAULT_VAL_OBSERVE_TX		0x00	
#define NRF24L01_REG_DEFAULT_VAL_RPD			0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_0	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_1	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_2	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_3	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_4	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_0	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_1	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_2	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_3	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_4	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P2		0xC3	
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P3		0xC4	
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P4		0xC5
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P5		0xC6
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_0		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_1		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_2		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_3		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_4		0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P0		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P1		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P2		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P3		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P4		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P5		0x00
#define NRF24L01_REG_DEFAULT_VAL_FIFO_STATUS	0x11
#define NRF24L01_REG_DEFAULT_VAL_DYNPD			0x00
#define NRF24L01_REG_DEFAULT_VAL_FEATURE		0x00

/* Configuration register*/
#define NRF24L01_MASK_RX_DR		6
#define NRF24L01_MASK_TX_DS		5
#define NRF24L01_MASK_MAX_RT	4
#define NRF24L01_EN_CRC			3
#define NRF24L01_CRCO			2
#define NRF24L01_PWR_UP			1
#define NRF24L01_PRIM_RX		0

/* Enable auto acknowledgment*/
#define NRF24L01_ENAA_P5		5
#define NRF24L01_ENAA_P4		4
#define NRF24L01_ENAA_P3		3
#define NRF24L01_ENAA_P2		2
#define NRF24L01_ENAA_P1		1
#define NRF24L01_ENAA_P0		0

/* Enable rx addresses */
#define NRF24L01_ERX_P5			5
#define NRF24L01_ERX_P4			4
#define NRF24L01_ERX_P3			3
#define NRF24L01_ERX_P2			2
#define NRF24L01_ERX_P1			1
#define NRF24L01_ERX_P0			0

/* Setup of address width */
#define NRF24L01_AW				0 //2 bits

/* Setup of auto re-transmission*/
#define NRF24L01_ARD			4 //4 bits
#define NRF24L01_ARC			0 //4 bits

/* RF setup register*/
#define NRF24L01_PLL_LOCK		4
#define NRF24L01_RF_DR_LOW		5
#define NRF24L01_RF_DR_HIGH		3
#define NRF24L01_RF_DR			3
#define NRF24L01_RF_PWR			1 //2 bits   

/* General status register */
#define NRF24L01_RX_DR			6
#define NRF24L01_TX_DS			5
#define NRF24L01_MAX_RT			4
#define NRF24L01_RX_P_NO		1 //3 bits
#define NRF24L01_TX_FULL		0

/* Transmit observe register */
#define NRF24L01_PLOS_CNT		4 //4 bits
#define NRF24L01_ARC_CNT		0 //4 bits

/* FIFO status*/
#define NRF24L01_TX_REUSE		6
#define NRF24L01_FIFO_FULL		5
#define NRF24L01_TX_EMPTY		4
#define NRF24L01_RX_FULL		1
#define NRF24L01_RX_EMPTY		0

//Dynamic length
#define NRF24L01_DPL_P0			0
#define NRF24L01_DPL_P1			1
#define NRF24L01_DPL_P2			2
#define NRF24L01_DPL_P3			3
#define NRF24L01_DPL_P4			4
#define NRF24L01_DPL_P5			5

/* Transmitter power*/
#define NRF24L01_M18DBM			0 //-18 dBm
#define NRF24L01_M12DBM			1 //-12 dBm
#define NRF24L01_M6DBM			2 //-6 dBm
#define NRF24L01_0DBM			3 //0 dBm

/* Data rates */
#define NRF24L01_2MBPS			0
#define NRF24L01_1MBPS			1
#define NRF24L01_250KBPS		2

/* Configuration */
#define NRF24L01_CONFIG			((1 << NRF24L01_EN_CRC) | (0 << NRF24L01_CRCO))

/* Instruction Mnemonics */
#define NRF24L01_REGISTER_MASK				0x1F

#define NRF24L01_READ_REGISTER_MASK(reg)	(0x00 | (NRF24L01_REGISTER_MASK & reg)) //Last 5 bits will indicate reg. address
#define NRF24L01_WRITE_REGISTER_MASK(reg)	(0x20 | (NRF24L01_REGISTER_MASK & reg)) //Last 5 bits will indicate reg. address
#define NRF24L01_R_RX_PAYLOAD_MASK			0x61
#define NRF24L01_W_TX_PAYLOAD_MASK			0xA0
#define NRF24L01_FLUSH_TX_MASK				0xE1
#define NRF24L01_FLUSH_RX_MASK				0xE2
#define NRF24L01_REUSE_TX_PL_MASK			0xE3
#define NRF24L01_ACTIVATE_MASK				0x50 
#define NRF24L01_R_RX_PL_WID_MASK			0x60
#define NRF24L01_NOP_MASK					0xFF

/* Flush FIFOs */
// #define NRF24L01_FLUSH_TX					do { NRF24L01_CSN_LOW; TM_SPI_Send(NRF24L01_SPI, NRF24L01_FLUSH_TX_MASK); NRF24L01_CSN_HIGH; } while (0)
// #define NRF24L01_FLUSH_RX					do { NRF24L01_CSN_LOW; TM_SPI_Send(NRF24L01_SPI, NRF24L01_FLUSH_RX_MASK); NRF24L01_CSN_HIGH; } while (0)

#define NRF24L01_TRANSMISSON_OK 			0
#define NRF24L01_MESSAGE_LOST   			1

#define NRF24L01_CHECK_BIT(reg, bit)       (reg & (1 << bit))

typedef struct {
	uint8_t PayloadSize;				//Payload size
	uint8_t Channel;					//Channel selected
	RF_OutputPower_t OutPwr;	//Output power
	RF_DataRate_t DataRate;	//Data rate
} RF_t;

/* Private functions */
void RF_InitPins(void);
void RF_WriteBit(uint8_t reg, uint8_t bit, uint8_t value);
uint8_t RF_ReadBit(uint8_t reg, uint8_t bit);
uint8_t RF_ReadRegister(uint8_t reg);
void RF_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t count);
void RF_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t count);
void RF_SoftwareReset(void);
uint8_t RF_RxFifoEmpty(void);

/* NRF structure */
static RF_t RF_Struct;

void RF_FlushTX(void) {
	uint8_t flushtx = NRF24L01_FLUSH_TX_MASK;
	NRF24L01_CSN_LOW;
	HAL_SPI_Transmit(&hspi1, &flushtx, 1, 5);
	NRF24L01_CSN_HIGH;
}

void RF_FlushRX(void) {
	uint8_t flushrx = NRF24L01_FLUSH_RX_MASK;
	NRF24L01_CSN_LOW;
	HAL_SPI_Transmit(&hspi1, &flushrx, 1, 5);
	NRF24L01_CSN_HIGH;
}


uint8_t RF_Init(uint8_t channel, uint8_t payload_size) {
	
		/* CSN high = disable SPI */
	NRF24L01_CSN_HIGH;
	
	/* CE low = disable TX/RX */
	NRF24L01_CE_LOW;
	
	/* Max payload is 32bytes */
	if (payload_size > 32) {
		payload_size = 32;
	}
	
	/* Fill structure */
	RF_Struct.Channel = !channel; /* Set channel to some different value for RF_SetChannel() function */
	RF_Struct.PayloadSize = payload_size;
	RF_Struct.OutPwr = RF_OutputPower_0dBm;
	RF_Struct.DataRate = RF_DataRate_2M;
	
	/* Reset nRF24L01+ to power on registers values */
	RF_SoftwareReset();
	
	/* Channel select */
	RF_SetChannel(channel);
	
	/* Set pipeline to max possible 32 bytes */
	RF_WriteRegister(NRF24L01_REG_RX_PW_P0, RF_Struct.PayloadSize); // Auto-ACK pipe
	RF_WriteRegister(NRF24L01_REG_RX_PW_P1, RF_Struct.PayloadSize); // Data payload pipe
	RF_WriteRegister(NRF24L01_REG_RX_PW_P2, RF_Struct.PayloadSize);
	RF_WriteRegister(NRF24L01_REG_RX_PW_P3, RF_Struct.PayloadSize);
	RF_WriteRegister(NRF24L01_REG_RX_PW_P4, RF_Struct.PayloadSize);
	RF_WriteRegister(NRF24L01_REG_RX_PW_P5, RF_Struct.PayloadSize);
	
	/* Set RF settings (2mbps, output power) */
	RF_SetRF(RF_Struct.DataRate, RF_Struct.OutPwr);
	
	/* Config register */
	RF_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG);
	
	/* Enable auto-acknowledgment for all pipes */
	RF_WriteRegister(NRF24L01_REG_EN_AA, 0x3F);
	
	/* Enable RX addresses */
	RF_WriteRegister(NRF24L01_REG_EN_RXADDR, 0x3F);

	/* Auto retransmit delay: 1000 (4x250) us and Up to 15 retransmit trials */
	RF_WriteRegister(NRF24L01_REG_SETUP_RETR, 0x4F);
	
	/* Dynamic length configurations: No dynamic length */
	RF_WriteRegister(NRF24L01_REG_DYNPD, (0 << NRF24L01_DPL_P0) | (0 << NRF24L01_DPL_P1) | (0 << NRF24L01_DPL_P2) | (0 << NRF24L01_DPL_P3) | (0 << NRF24L01_DPL_P4) | (0 << NRF24L01_DPL_P5));
	
	/* Clear FIFOs */
	RF_FlushTX();
	RF_FlushRX();
	
	uint8_t st = RF_GetStatus();
	
	/* Clear interrupts */
	RF_Clear_Interrupts();
	
	/* Go to RX mode */
	RF_PowerUpRx();
	
	st = RF_GetStatus();
	
	/* Return OK */
	return 1;
}


void RF_SetMyAddress(uint8_t *adr) {
	NRF24L01_CE_LOW;
	RF_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, adr, 5);
	NRF24L01_CE_HIGH;
}

void RF_SetTxAddress(uint8_t *adr) {
	RF_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, adr, 5);
	RF_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, adr, 5);
}

void RF_WriteBit(uint8_t reg, uint8_t bit, uint8_t value) {
	uint8_t tmp;
	/* Read register */
	tmp = RF_ReadRegister(reg);
	/* Make operation */
	if (value) {
		tmp |= 1 << bit;
	} else {
		tmp &= ~(1 << bit);
	}
	/* Write back */
	RF_WriteRegister(reg, tmp);
}

uint8_t RF_ReadBit(uint8_t reg, uint8_t bit) {
	uint8_t tmp;
	tmp = RF_ReadRegister(reg);
	if (!NRF24L01_CHECK_BIT(tmp, bit)) {
		return 0;
	}
	return 1;
}

uint8_t RF_ReadRegister(uint8_t reg) {
	uint8_t value;
	
	uint8_t regist = NRF24L01_READ_REGISTER_MASK(reg);
	NRF24L01_CSN_LOW;
	HAL_SPI_TransmitReceive(&hspi1, &regist, &value, 1, 5);
	NRF24L01_CSN_HIGH;
	
	return value;
}

void RF_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t count) {
	uint8_t regist = NRF24L01_READ_REGISTER_MASK(reg);
	
	NRF24L01_CSN_LOW;
	HAL_SPI_Transmit(&hspi1, &regist, 1, 5);
	HAL_SPI_Receive(&hspi1, data, count, 5);
	NRF24L01_CSN_HIGH;
}

void RF_WriteRegister(uint8_t reg, uint8_t value) {
	uint8_t regist = NRF24L01_WRITE_REGISTER_MASK(reg);
	
	NRF24L01_CSN_LOW;
	uint32_t res = HAL_SPI_Transmit(&hspi1, &regist, 1, 5);
	res = HAL_SPI_Transmit(&hspi1, &value, 1, 5);
	NRF24L01_CSN_HIGH;
}

void RF_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t count) {
	uint8_t regist = NRF24L01_WRITE_REGISTER_MASK(reg);
	
	NRF24L01_CSN_LOW;
	HAL_SPI_Transmit(&hspi1, &regist, 1, 5);
	HAL_SPI_Transmit(&hspi1, data, count, 5);
	NRF24L01_CSN_HIGH;
}

void RF_PowerUpTx(void) {
	RF_Clear_Interrupts();
	RF_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG | (0 << NRF24L01_PRIM_RX) | (1 << NRF24L01_PWR_UP));
}

void RF_PowerUpRx(void) {
	/* Disable RX/TX mode */
	NRF24L01_CE_LOW;
	/* Clear RX buffer */
	RF_FlushRX();
	/* Clear interrupts */
	RF_Clear_Interrupts();
	/* Setup RX mode */
	RF_WriteRegister(NRF24L01_REG_CONFIG, NRF24L01_CONFIG | 1 << NRF24L01_PWR_UP | 1 << NRF24L01_PRIM_RX);
	/* Start listening */
	NRF24L01_CE_HIGH;
}

void RF_PowerDown(void) {
	NRF24L01_CE_LOW;
	RF_WriteBit(NRF24L01_REG_CONFIG, NRF24L01_PWR_UP, 0);
}

void RF_Transmit(uint8_t *data) {
	uint8_t count = RF_Struct.PayloadSize;
	uint8_t value = NRF24L01_W_TX_PAYLOAD_MASK;
	/* Chip enable put to low, disable it */
	NRF24L01_CE_LOW;
	
	/* Go to power up tx mode */
	RF_PowerUpTx();
	
	/* Clear TX FIFO from NRF24L01+ */
	RF_FlushTX();
	
	uint8_t status = RF_GetStatus();
	/* Send payload to nRF24L01+ */
	NRF24L01_CSN_LOW;
	/* Send write payload command */
	HAL_SPI_Transmit(&hspi1, &value, 1, 5);
	// TM_SPI_Send(NRF24L01_SPI, NRF24L01_W_TX_PAYLOAD_MASK);
	/* Fill payload with data*/
	HAL_SPI_Transmit(&hspi1, data, count, 5);
	// TM_SPI_WriteMulti(NRF24L01_SPI, data, count);
	/* Disable SPI */
	NRF24L01_CSN_HIGH;
	
	/* Send data! */
	NRF24L01_CE_HIGH;
}

void RF_GetData(uint8_t* data) {
	/* Pull down chip select */
	NRF24L01_CSN_LOW;
	uint8_t value = NRF24L01_R_RX_PAYLOAD_MASK;
	/* Send read payload command*/
	HAL_SPI_Transmit(&hspi1, &value, 1, 5);
	/* Read payload */
	HAL_SPI_Receive(&hspi1, data, RF_Struct.PayloadSize, 5);
	// TM_SPI_SendMulti(NRF24L01_SPI, data, data, RF_Struct.PayloadSize);
	/* Pull up chip select */
	NRF24L01_CSN_HIGH;
	
	/* Reset status register, clear RX_DR interrupt flag */
	RF_WriteRegister(NRF24L01_REG_STATUS, (1 << NRF24L01_RX_DR));
}

uint8_t RF_DataReady(void) {
	uint8_t status = RF_GetStatus();
	
	if (NRF24L01_CHECK_BIT(status, NRF24L01_RX_DR)) {
		return 1;
	}
	return !RF_RxFifoEmpty();
}

uint8_t RF_RxFifoEmpty(void) {
	uint8_t reg = RF_ReadRegister(NRF24L01_REG_FIFO_STATUS);
	return NRF24L01_CHECK_BIT(reg, NRF24L01_RX_EMPTY);
}

uint8_t RF_GetStatus(void) {
	uint8_t status;
	uint8_t dummy = NRF24L01_NOP_MASK;
	
	NRF24L01_CSN_LOW;
	/* First received byte is always status register */
	 HAL_SPI_TransmitReceive(&hspi1, &dummy, &status, 1, 5);
	// status = TM_SPI_Send(NRF24L01_SPI, NRF24L01_NOP_MASK);
	/* Pull up chip select */
	NRF24L01_CSN_HIGH;
	
	return status;
}

RF_Transmit_Status_t RF_GetTransmissionStatus(void) {
	uint8_t status = RF_GetStatus();
	if (NRF24L01_CHECK_BIT(status, NRF24L01_TX_DS)) {
		/* Successfully sent */
		return RF_Transmit_Status_Ok;
	} else if (NRF24L01_CHECK_BIT(status, NRF24L01_MAX_RT)) {
		/* Message lost */
		return RF_Transmit_Status_Lost;
	}
	
	/* Still sending */
	return RF_Transmit_Status_Sending;
}

void RF_SoftwareReset(void) {
	uint8_t data[5];
	
	RF_WriteRegister(NRF24L01_REG_CONFIG, 		NRF24L01_REG_DEFAULT_VAL_CONFIG);
	RF_WriteRegister(NRF24L01_REG_EN_AA,		NRF24L01_REG_DEFAULT_VAL_EN_AA);
	RF_WriteRegister(NRF24L01_REG_EN_RXADDR, 	NRF24L01_REG_DEFAULT_VAL_EN_RXADDR);
	RF_WriteRegister(NRF24L01_REG_SETUP_AW, 	NRF24L01_REG_DEFAULT_VAL_SETUP_AW);
	RF_WriteRegister(NRF24L01_REG_SETUP_RETR, 	NRF24L01_REG_DEFAULT_VAL_SETUP_RETR);
	RF_WriteRegister(NRF24L01_REG_RF_CH, 		NRF24L01_REG_DEFAULT_VAL_RF_CH);
	RF_WriteRegister(NRF24L01_REG_RF_SETUP, 	NRF24L01_REG_DEFAULT_VAL_RF_SETUP);
	RF_WriteRegister(NRF24L01_REG_STATUS, 		NRF24L01_REG_DEFAULT_VAL_STATUS);
	RF_WriteRegister(NRF24L01_REG_OBSERVE_TX, 	NRF24L01_REG_DEFAULT_VAL_OBSERVE_TX);
	RF_WriteRegister(NRF24L01_REG_RPD, 		NRF24L01_REG_DEFAULT_VAL_RPD);
	
	//P0
	data[0] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_4;
	RF_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, data, 5);
	
	//P1
	data[0] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_4;
	RF_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, data, 5);
	
	RF_WriteRegister(NRF24L01_REG_RX_ADDR_P2, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P2);
	RF_WriteRegister(NRF24L01_REG_RX_ADDR_P3, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P3);
	RF_WriteRegister(NRF24L01_REG_RX_ADDR_P4, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P4);
	RF_WriteRegister(NRF24L01_REG_RX_ADDR_P5, 	NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P5);
	
	//TX
	data[0] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_0;
	data[1] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_1;
	data[2] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_2;
	data[3] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_3;
	data[4] = NRF24L01_REG_DEFAULT_VAL_TX_ADDR_4;
	RF_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, data, 5);
	
	RF_WriteRegister(NRF24L01_REG_RX_PW_P0, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P0);
	RF_WriteRegister(NRF24L01_REG_RX_PW_P1, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P1);
	RF_WriteRegister(NRF24L01_REG_RX_PW_P2, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P2);
	RF_WriteRegister(NRF24L01_REG_RX_PW_P3, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P3);
	RF_WriteRegister(NRF24L01_REG_RX_PW_P4, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P4);
	RF_WriteRegister(NRF24L01_REG_RX_PW_P5, 	NRF24L01_REG_DEFAULT_VAL_RX_PW_P5);
	RF_WriteRegister(NRF24L01_REG_FIFO_STATUS, NRF24L01_REG_DEFAULT_VAL_FIFO_STATUS);
	RF_WriteRegister(NRF24L01_REG_DYNPD, 		NRF24L01_REG_DEFAULT_VAL_DYNPD);
	RF_WriteRegister(NRF24L01_REG_FEATURE, 	NRF24L01_REG_DEFAULT_VAL_FEATURE);
}

uint8_t RF_GetRetransmissionsCount(void) {
	/* Low 4 bits */
	return RF_ReadRegister(NRF24L01_REG_OBSERVE_TX) & 0x0F;
}

void RF_SetChannel(uint8_t channel) {
	if (channel <= 125 && channel != RF_Struct.Channel) {
		/* Store new channel setting */
		RF_Struct.Channel = channel;
		/* Write channel */
		RF_WriteRegister(NRF24L01_REG_RF_CH, channel);
	}
}

void RF_SetRF(RF_DataRate_t DataRate, RF_OutputPower_t OutPwr) {
	uint8_t tmp = 0;
	RF_Struct.DataRate = DataRate;
	RF_Struct.OutPwr = OutPwr;
	
	if (DataRate == RF_DataRate_2M) {
		tmp |= 1 << NRF24L01_RF_DR_HIGH;
	} else if (DataRate == RF_DataRate_250k) {
		tmp |= 1 << NRF24L01_RF_DR_LOW;
	}
	/* If 1Mbps, all bits set to 0 */
	
	if (OutPwr == RF_OutputPower_0dBm) {
		tmp |= 3 << NRF24L01_RF_PWR;
	} else if (OutPwr == RF_OutputPower_M6dBm) {
		tmp |= 2 << NRF24L01_RF_PWR;
	} else if (OutPwr == RF_OutputPower_M12dBm) {
		tmp |= 1 << NRF24L01_RF_PWR;
	}
	
	RF_WriteRegister(NRF24L01_REG_RF_SETUP, tmp);
}

uint8_t RF_Read_Interrupts(RF_IRQ_t* IRQ) {
	IRQ->Status = RF_GetStatus();
	return IRQ->Status;
}

void RF_Clear_Interrupts(void) {
	RF_WriteRegister(0x07, 0x70);
}

