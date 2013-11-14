/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.manager;

import de.juehvtech.smarthomeproject.heater.actor.Heater;
import de.juehvtech.smarthomeproject.heater.app.Properties;
import de.juehvtech.smarthomeproject.heater.heatingplan.HeatingPlan;
import de.juehvtech.smarthomeproject.heater.heatingplan.HeatingPlanReader;
import de.juehvtech.smarthomeproject.heater.sensor.Temperature;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Jens
 */
public class MainRunner implements Runnable {

    private static int CYCLES = 3;

    @Override
    public void run() {
        // Read Heating Plan
        HeatingPlanReader reader = new HeatingPlanReader(Properties.HEATING_PLAN_FILE);
        HeatingPlan plan = reader.read();

        if (plan != null) {
            try {
                // Execute Plan
                Temperature sensor = new Temperature();
                Heater heater = new Heater();
                if (sensor.isWarmEnough(plan.getActiveTemperature())) {
                    // Turn Heater off
                    heater.turnOff();
                } else {
                    // Turn Heater on
                    heater.turnOn();
                }
            } catch (IOException ex) {
                Logger.getLogger(MainRunner.class.getName())
                        .log(Level.SEVERE, "Error in cycle.", ex);
            }
        }
    }
}
