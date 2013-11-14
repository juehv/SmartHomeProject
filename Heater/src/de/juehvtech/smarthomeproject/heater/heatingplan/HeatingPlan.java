/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.heatingplan;

import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.logging.Logger;

/**
 *
 * @author Jens
 */
public class HeatingPlan {

    private final List<List<Float>> plan;

    public HeatingPlan(List<List<Float>> plan) {
        this.plan = plan;
    }

    public float getActiveTemperature() {
        GregorianCalendar now = new GregorianCalendar();
        int day = now.get(Calendar.DAY_OF_WEEK);
        int hour = now.get(Calendar.HOUR_OF_DAY);
        int minute = now.get(Calendar.MINUTE);

        // choose day
        List<Float> planForToday;
        switch (day) {
            case Calendar.MONDAY:
                planForToday = plan.get(0);
                break;
            case Calendar.TUESDAY:
                planForToday = plan.get(1);
                break;
            case Calendar.WEDNESDAY:
                planForToday = plan.get(2);
                break;
            case Calendar.THURSDAY:
                planForToday = plan.get(3);
                break;
            case Calendar.FRIDAY:
                planForToday = plan.get(4);
                break;
            case Calendar.SATURDAY:
                planForToday = plan.get(5);
                break;
            case Calendar.SUNDAY:
                planForToday = plan.get(6);
                break;
            default:
                // Fallback for error
                planForToday = plan.get(0);
                Logger.getLogger(HeatingPlan.class.getName()).warning(
                        "Take fallback plan!");
                break;
        }

        // choose timeslot
        int slot = hour * 2;
        if (minute > 29) {
            slot++;
        }
        float temparature = planForToday.get(slot);

        // return value 
        return temparature;
    }
}
