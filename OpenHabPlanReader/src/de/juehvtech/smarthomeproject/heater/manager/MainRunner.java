/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.manager;

import de.juehvtech.smarthomeproject.heater.app.Properties;
import de.juehvtech.smarthomeproject.heater.heatingplan.HeatingPlan;
import de.juehvtech.smarthomeproject.heater.heatingplan.HeatingPlanReader;

/**
 *
 * @author Jens
 */
public class MainRunner implements Runnable {

    @Override
    public void run() {
        // Read Heating Plan
        HeatingPlanReader reader = new HeatingPlanReader(Properties.HEATING_PLAN_FILE);
        HeatingPlan plan = reader.read();

        if (plan != null) {
            System.out.print(plan.getActiveTemperature());
        } else {
            System.out.print("18");
        }
    }
}
