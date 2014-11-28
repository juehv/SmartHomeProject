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
    public static final String HEATING_PLAN_FILE = "heating-plan-file";
    // Short Tags
    public static final char SHORT_HELP = 'h';
    public static final char SHORT_VERSION = 'v';
    public static final char SHORT_HEATING_PLAN_FILE = 'p';
    // Help Text
    public static final String TXT_HELP = "Shows this page.";
    public static final String TXT_VERSION = "Shows the version information.";
    public static final String TXT_HEATING_PLAN_FILE = "The heating plan file.";
    // version string   
    public static final String VERSION_STRING = "CSV Heating Plan Reader (for usage with OpenHAB)\n"
            + "Version: 0.2 Beta (11/2014)\n"
            + "License: GPLv3\n"
            + "http://www.juehv-tech.de";
}
