/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.sensor;

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * FOr more info
 * https://github.com/adafruit/Adafruit_MPL115A2/blob/master/Adafruit_MPL115A2.cpp
 * https://github.com/adafruit/Adafruit_MPL115A2/blob/master/Adafruit_MPL115A2.h
 * http://www.adafruit.com/datasheets/MPL115A2.pdf
 *
 * @author Jens
 */
public class Temperature {

    private static float MAGIC_CORRECTION_FACTOR = 3.0f;
    private final I2CDevice tempSensor;
    private int a0Coeff;
    private int b1Coeff;
    private int b2Coeff;
    private int c12Coeff;

    public Temperature() throws IOException {
        this.tempSensor = I2CFactory.getInstance(I2CBus.BUS_1)
                .getDevice(0x60);
        readCoefficients();
    }

    private void readCoefficients() {
        try {
            // not needed now...
            a0Coeff = tempSensor.read(0x04) << 8;
            a0Coeff |= tempSensor.read(0x05);
            b1Coeff = tempSensor.read(0x06) << 8;
            b1Coeff |= tempSensor.read(0x07);
            b2Coeff = tempSensor.read(0x08) << 8;
            b2Coeff |= tempSensor.read(0x09);
            c12Coeff = tempSensor.read(0x0A) << 8;
            c12Coeff |= tempSensor.read(0x0B);
            c12Coeff = c12Coeff >> 2;

        } catch (IOException ex) {
            Logger.getLogger(Temperature.class.getName())
                    .log(Level.SEVERE, "Error while reading coefficients",
                    ex);
        }
    }

    public float readTemp() {
        try {
            // start conversation
            tempSensor.write(0x12, (byte) 0x00);
            // wait for a/d hardware
            Thread.sleep(5);

            int[] values = new int[4];
            // Pressure (not needed now)
            values[0] = tempSensor.read(0x00);
            values[1] = tempSensor.read(0x01);
            // Temperature
            values[2] = tempSensor.read(0x02);
            values[3] = tempSensor.read(0x03);
            int temp = ((values[2] << 8) | values[3]) >> 6;
            // calculate the magic temperature
            // http://www.adafruit.com/forums/viewtopic.php?f=19&t=41347
            float magicTemp = ((temp - 498.0F) / -5.35F + 25.0F) + MAGIC_CORRECTION_FACTOR;

            // debug
            System.out.println("Values " + Arrays.toString(values) + " " + temp);
            System.out.println("Temp " + magicTemp);

            // return value
            return magicTemp;
        } catch (IOException | InterruptedException ex) {
            Logger.getLogger(Temperature.class.getName())
                    .log(Level.SEVERE, "Error while reading values.",
                    ex);
        }
        return -1;
    }

    public boolean isWarmEnough(float activeTemperature) {
        int runs = 15;
        float temp = 0;
        for (int i = 0; i < runs; i++) {
            temp += readTemp();
        }
        temp /= runs;
        return temp >= activeTemperature;
    }
}
