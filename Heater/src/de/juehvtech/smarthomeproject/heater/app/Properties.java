/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.app;

/**
 *
 * @author Jens
 */
public final class Properties {

    private Properties() {
        // empty
    }
    public static final String LOCK_FILE_NAME = "heating-manager.lock";
    public static String LOCK_FILE_PATH = "../";
    public static String HEATING_PLAN_FILE = "";
    public static float MANUAL_TEMPERATURE = 22F;
}