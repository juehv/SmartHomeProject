/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.app;

/**
 *
 * @author Jens
 */
public final class ParserTags {

    private ParserTags() {
        // empty
    }
    // Tags
    public static final String HELP = "help";
    public static final String VERSION = "version";
    public static final String LOCK_FILE_PATH = "lock-path";
    public static final String HEATING_PLAN_FILE = "heating-plan-file";
    public static final String INIT_IO = "init-io";
    public static final String MANUAL_TEMP = "manual-temperature";
    // Short Tags
    public static final char SHORT_HELP = 'h';
    public static final char SHORT_VERSION = 'v';
    public static final char SHORT_LOCK_FILE_PATH = 'l';
    public static final char SHORT_HEATING_PLAN_FILE = 'p';
    public static final char SHORT_INIT_IO = 'I';
    public static final char SHORT_MANUAL_TEMP = 'T';
    // Help Text
    public static final String TXT_HELP = "Shows this page.";
    public static final String TXT_VERSION = "Shows the version information.";
    public static final String TXT_LOCK_FILE_PATH = "Path to the lock directory. Usually /var/lock/. Default is ./";
    public static final String TXT_HEATING_PLAN_FILE = "The heating plan file.";
    public static final String TXT_INIT_IO = "If this flag is set, the program will init the needed IO ports and exit.";
    public static final String TXT_MANUAL_TEMP = "Sets the temperature for manual heater activation (in °C). Default is 22.";
    // version string   
    public static final String VERSION_STRING = "Heating Manager (SmartHomePi)\n"
            + "Version: 0.6 Alpha (11/2013)\n"
            + "License: GPLv3\n"
            + "http://www.juehv-tech.de";
}
