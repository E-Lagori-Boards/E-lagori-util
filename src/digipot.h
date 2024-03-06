#ifndef I2C_DIGIPOT_H
#define I2C_DIGIPOT_H

// #include "driver/i2c_master.h"
#include "driver/i2c.h"


struct digipot{
        unsigned addr;
        // i2c_master_dev_handle_t *i2c;
        i2c_port_t *i2c;
};

extern "C" int digipot_chgBoardaddr(struct digipot *,int);
extern "C" void digipot_init(struct digipot *, int, i2c_port_t *);
extern "C" int digipot_check_i2c(struct digipot *, int);
extern "C" int digipot_Command16(struct digipot *);
extern "C" void digipot_Write_RDAC(struct digipot *,bool, int);
extern "C" int digipot_Readback_RDAC(struct digipot *,bool);
extern "C" void digipot_Write_EEPROM(struct digipot *,bool, int);
#endif
