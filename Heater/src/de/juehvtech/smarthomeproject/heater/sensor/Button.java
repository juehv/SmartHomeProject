/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.sensor;

import com.pi4j.io.gpio.GpioFactory;
import com.pi4j.io.gpio.GpioPinDigitalInput;
import com.pi4j.io.gpio.PinPullResistance;
import com.pi4j.io.gpio.RaspiPin;

/**
 *
 * @author Jens
 */
public class Button {
     private final GpioPinDigitalInput buttonPin;

    public Button() {
        buttonPin = GpioFactory.getInstance().provisionDigitalInputPin(
                RaspiPin.GPIO_01, PinPullResistance.PULL_UP);
    }

    public boolean isPressed() {
        return buttonPin.isLow();
    }
}
