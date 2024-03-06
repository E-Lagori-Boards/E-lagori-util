
#include "digipot.h"
#include "driver/i2c.h"

int digipot_chgBoardaddr(struct digipot *p,int address){
switch (address)
{
    case 11: p->addr = 0b0100000;break;
    case 21: p->addr = 0b0100010;break;
    case 01: p->addr = 0b0100011;break;
    case 12: p->addr = 0b0101000;break;
    case 22: p->addr = 0b0101010;break;
    case 02: p->addr = 0b0101011;break;
    case 10: p->addr = 0b0101100;break;
    case 20: p->addr = 0b0101110;break;
    case 00: p->addr = 0b0101111;break;
    default: p->addr=0;break;
}
return (p->addr);
}

// void digipot_init(struct digipot *p, int address, i2c_master_bus_handle_t *i2c)
// {
//   chgBoardaddr(address);
//   i2c_device_config_t devcfg = {
//         .dev_addr_length = I2C_ADDR_BIT_LEN_7
//         .device_address = p->addr,
//         .scl_speed_hz = 100000
//     };
//     ESP_ERROR_CHECK(i2c_master_bus_add_device(*i2c, &devcfg, p->i2c));
// };
void digipot_init(struct digipot *p, int address, i2c_port_t *i2c){
  digipot_chgBoardaddr(p,address);
  p->i2c = i2c;
};

// int digipot_check_i2c(struct digipot *p, int data){
//     uint8_t txbuffer[2],rxbuffer[2];
//     txbuffer[0] = 0b00110000;
//     txbuffer[1] = data;
//     i2c_master_transmit(p->i2c,txbuffer,2,-1);
//     i2c_master_transmit_receive(p->i2c, p->txbuffer, 2, rxbuffer, 2, -1);
//     read_data = ((*((int*)&rxbuffer) == 19) || (*((int*)&rxbuffer) == 3))? 1:0;
//     return read_data;
// }

// int digipot_Command16(struct digipot *p){
//     p->txbuffer[0] = 0B11010000;
//     p->txbuffer[1] = 0B00000011;
//     i2c_master_transmit(p->i2c,p->txbuffer,2,-1); 
//     return check_i2c(2);
// }

// int digipot_Write_RDAC(struct digipot *p,bool RDACno, int D_set){
//     Command16();
//     d_bit = D_set;
//     p->txbuffer[0] = (RDACno)?0b00010001:0b00010000;
//     p->txbuffer[1] = (p->d_bit);
//     i2c_master_transmit(p->i2c,p->txbuffer,2,-1);    
// }

// int digipot_Readback_RDAC(struct digipot *p,bool RDACno){
//         uint16_t read_data;
//         p->d_bit = 3;  
//         p->txbuffer[0] = (p->RDACno)?0b00110001:0b00110000;
//         p->txbuffer[1] = p->d_bit ;
//         i2c_master_transmit_receive(p->i2c, p->txbuffer, 2, &read_data, 2, -1);
//         return read_data;
//         }

// void digipot_Write_EEPROM(struct digipot *p,bool RDACno, int D_set){   
//     Command16();
//     p->d_bit = D_set;
//     p->txbuffer[0] = (RDACno)?0b10000001:0b10000000;
//     p->txbuffer[1] = (p->d_bit);
//     i2c_master_transmit(p->i2c,p->txbuffer,2,-1);
// }

int digipot_check_i2c(struct digipot *p, int data){
    uint8_t txbuffer[2];
    uint16_t rxbuffer = 0;
    txbuffer[0] = 0b00110000;
    txbuffer[1] = data;
    ESP_ERROR_CHECK(i2c_master_write_read_device(*(p->i2c), p->addr, txbuffer, 2, (uint8_t*)&rxbuffer, 2, 10/portTICK_PERIOD_MS));
    return ((rxbuffer == 19) || (rxbuffer == 3))? 1:0;
}

int digipot_Command16(struct digipot *p){
    uint8_t txbuffer[2];
    txbuffer[0] = 0B11010000;
    txbuffer[1] = 0B00000011;
    ESP_ERROR_CHECK(i2c_master_write_to_device(*(p->i2c),p->addr, txbuffer,2,-1)); 
    return digipot_check_i2c(p,2);
}

void digipot_Write_RDAC(struct digipot *p,bool RDACno, int D_set){
    uint8_t txbuffer[2];
    digipot_Command16(p);
    txbuffer[0] = (RDACno)?0b00010001:0b00010000;
    txbuffer[1] =  D_set;
    ESP_ERROR_CHECK(i2c_master_write_to_device(*(p->i2c),p->addr, txbuffer,2,-1));     
}

int digipot_Readback_RDAC(struct digipot *p,bool RDACno){
    uint8_t txbuffer[2];
    uint16_t rxbuffer;
    txbuffer[0] = (RDACno)?0b00110001:0b00110000;
    txbuffer[1] = 3;
    ESP_ERROR_CHECK(i2c_master_write_read_device(*(p->i2c), p->addr, txbuffer, 2, (uint8_t*)&rxbuffer, 2, 10/portTICK_PERIOD_MS));
    return rxbuffer;
}

void digipot_Write_EEPROM(struct digipot *p,bool RDACno, int D_set){   
    uint8_t txbuffer[2];
    digipot_Command16(p);
    txbuffer[0] = (RDACno)?0b10000001:0b10000000;
    txbuffer[1] = D_set;
    ESP_ERROR_CHECK(i2c_master_write_to_device(*(p->i2c), p->addr, txbuffer,2,-1));
}