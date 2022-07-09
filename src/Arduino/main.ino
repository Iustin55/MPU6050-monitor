#include <Wire.h>

#define BAUD_RATE 19200
#define I2C_ADDR_MPU 0b1101000

/* Registers definitions */
#define PWR_MGMT_REG 0x6B
#define GYR_CNFG_REG 0x1B
#define ACC_CNFG_REG 0x1C
#define GYR_OUT_REG 0x43
#define ACC_OUT_REG 0x3B

/* Raw accelerometer data */
static long acc_x_raw;
static long acc_y_raw;
static long acc_z_raw;

/* Raw gyroscope data */
static long gyr_x_raw;
static long gyr_y_raw;
static long gyr_z_raw;

/* Normalized accelerometer data (g) */
static float acc_x_g;
static float acc_y_g;
static float acc_z_g;

/* Normalized gyroscope data (rot) */
float gyr_x_rot;
float gyr_y_rot;
float gyr_z_rot;

void setup() 
{
    Serial.begin( BAUD_RATE );
    Wire.begin();
    MPU_init();
}

void loop() 
{
    get_acc_data();
    get_gyr_data();
    send_data_ser();
    delay( 100 );
}

void MPU_init(){
    /* Modifying the values in the power management registry to change the state from SLEEP to ACTIVE */
    Wire.beginTransmission( I2C_ADDR_MPU ); 
    Wire.write( PWR_MGMT_REG ); 
    Wire.write( 0b00000000 ); 
    Wire.endTransmission();  

    Wire.beginTransmission( I2C_ADDR_MPU ); 
    Wire.write( GYR_CNFG_REG );
    Wire.write( 0x00000000 );
    Wire.endTransmission(); 

    Wire.beginTransmission( I2C_ADDR_MPU );
    Wire.write( ACC_CNFG_REG );
    Wire.write( 0b00000000 );
    Wire.endTransmission(); 
}

void get_acc_data() 
{   
    Wire.beginTransmission( I2C_ADDR_MPU );
    Wire.write( ACC_OUT_REG );
    Wire.endTransmission();
    Wire.requestFrom( I2C_ADDR_MPU, 6 );

    while( Wire.available() < 6 );
    acc_x_raw = Wire.read() << 8 | Wire.read(); 
    acc_y_raw = Wire.read() << 8 | Wire.read();
    acc_z_raw = Wire.read() << 8 | Wire.read();

    acc_x_g = acc_x_raw / 16384.0;
    acc_y_g = acc_y_raw / 16384.0;
    acc_z_g = acc_z_raw / 16384.0;
}

void get_gyr_data() 
{
    Wire.beginTransmission( I2C_ADDR_MPU );
    Wire.write( GYR_OUT_REG );
    Wire.endTransmission();
    Wire.requestFrom( I2C_ADDR_MPU, 6 );

    while( Wire.available() < 6 );
    gyr_x_raw = ( Wire.read() << 8 | Wire.read() );
    gyr_y_raw = ( Wire.read() << 8 | Wire.read() );
    gyr_z_raw = ( Wire.read() << 8 | Wire.read() );

    gyr_x_rot = gyr_x_raw / 131.0;
    gyr_y_rot = gyr_y_raw / 131.0;
    gyr_z_rot = gyr_z_raw / 131.0;
}

void send_data_ser() 
{
    /* First three values are the gyroscope output */
    Serial.print( gyr_x_rot );
    Serial.print( " " );
    Serial.print( gyr_y_rot );
    Serial.print( " " );
    Serial.print( gyr_z_rot );
    Serial.print( " " );

    /* Next three values are the accelerometer output */
    Serial.print( acc_x_g );
    Serial.print( " " );
    Serial.print( acc_y_g );
    Serial.print( " " );
    Serial.print( acc_z_g );
    Serial.print( " " );
    Serial.println();
}
