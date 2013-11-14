/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.actor;

import com.pi4j.io.gpio.GpioFactory;
import com.pi4j.io.gpio.GpioPinDigitalOutput;
import com.pi4j.io.gpio.PinState;
import com.pi4j.io.gpio.RaspiPin;

/**
 * Wrapper for relais access
 *
 * @author Jens
 */
public class Heater {

    private final GpioPinDigitalOutput relaisPin;

    public Heater() {
        relaisPin = GpioFactory.getInstance().provisionDigitalOutputPin(
                RaspiPin.GPIO_05 /*RaspiPin.GPIO_05*/, "heater", PinState.HIGH);
    }

    public void turnOff() {
        relaisPin.high();
    }

    public void turnOn() {
        relaisPin.low();
    }
}
