/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.app;

import com.martiansoftware.jsap.FlaggedOption;
import com.martiansoftware.jsap.JSAP;
import com.martiansoftware.jsap.JSAPException;
import com.martiansoftware.jsap.JSAPResult;
import com.martiansoftware.jsap.Switch;
import java.io.File;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Jens
 */
public class ArgumentParser {

    public static final int RESULT_OK = 0;
    public static final int RESULT_HELP = 1;
    public static final int RESULT_FILE_NOT_FOUND = 2;
    public static final int RESULT_WRONG_CMD = 3;
    public static final int RESULT_PARSER_EXCEPTION = 4;
    public static final int RESULT_OTHER_ERROR = 5;
    public static final int RESULT_SETUP = 6;

    private ArgumentParser() {
        //empty
    }

    public static int parse(String[] args) {
        try {
            JSAP jsap = new JSAP();
            // Example:
            // FlaggedOption opt1 = new FlaggedOption("count")
            // .setStringParser(JSAP.INTEGER_PARSER)
            // .setDefault("1")
            // .setRequired(true)
            // .setShortFlag('n')
            // .setLongFlag(JSAP.NO_LONGFLAG);
            // jsap.registerParameter(opt1);
            //
            // Switch sw1 = new Switch("verbose")
            // .setShortFlag('v')
            // jsap.registerParameter(sw1);
            // jsap.registerParameter(sw1);

            // help
            Switch help = new Switch(ParserTags.HELP);
            help.setShortFlag('h')
                    .setLongFlag("help")
                    .setHelp("Shows this page.");
            jsap.registerParameter(help);

            // version info
            Switch version = new Switch(ParserTags.VERSION);
            version.setShortFlag(JSAP.NO_SHORTFLAG)
                    .setLongFlag("version")
                    .setHelp("Shows the version information.");
            jsap.registerParameter(version);

            // lock file
            FlaggedOption lockFilePath = new FlaggedOption(ParserTags.LOCK_FILE_PATH);
            lockFilePath.setStringParser(JSAP.STRING_PARSER)
                    .setShortFlag(ParserTags.SHORT_LOCK_FILE_PATH)
                    .setLongFlag(ParserTags.LOCK_FILE_PATH)
                    .setRequired(false)
                    .setList(false)
                    .setHelp(ParserTags.TXT_LOCK_FILE_PATH);
            jsap.registerParameter(lockFilePath);

            // backup plan file
            FlaggedOption heatingPlan = new FlaggedOption(ParserTags.HEATING_PLAN_FILE);
            heatingPlan.setStringParser(JSAP.STRING_PARSER)
                    .setShortFlag(ParserTags.SHORT_HEATING_PLAN_FILE)
                    .setLongFlag(ParserTags.HEATING_PLAN_FILE)
                    .setRequired(true)
                    .setList(false)
                    .setHelp(ParserTags.TXT_HEATING_PLAN_FILE);
            jsap.registerParameter(heatingPlan);

            // Init IO
            Switch initIO = new Switch(ParserTags.INIT_IO);
            initIO.setShortFlag(ParserTags.SHORT_INIT_IO)
                    .setLongFlag(ParserTags.INIT_IO)
                    .setHelp(ParserTags.TXT_INIT_IO);
            jsap.registerParameter(initIO);
            
            // backup plan file
            FlaggedOption manualTemp = new FlaggedOption(ParserTags.MANUAL_TEMP);
            manualTemp.setStringParser(JSAP.STRING_PARSER)
                    .setShortFlag(ParserTags.SHORT_MANUAL_TEMP)
                    .setLongFlag(ParserTags.MANUAL_TEMP)
                    .setRequired(false)
                    .setList(false)
                    .setHelp(ParserTags.TXT_MANUAL_TEMP);
            jsap.registerParameter(manualTemp);

            // get results
            JSAPResult config = jsap.parse(args);

            if (config.success()) { // manual args
                Properties.LOCK_FILE_PATH = config.getString(ParserTags.LOCK_FILE_PATH);
                Properties.HEATING_PLAN_FILE = config.getString(ParserTags.HEATING_PLAN_FILE);
                if ((new File(Properties.LOCK_FILE_PATH)).exists()
                        && (new File(Properties.LOCK_FILE_PATH)).isDirectory()
                        && (new File(Properties.LOCK_FILE_PATH)).canWrite()
                        && (new File(Properties.HEATING_PLAN_FILE)).exists()
                        && (new File(Properties.HEATING_PLAN_FILE)).canRead()) {
                    return RESULT_OK;
                } else {
                    System.err.println();
                    System.err.println("Error with arguments. Check if "
                            + "directoies exist and are writeable.");
                    System.err.println();
                    return RESULT_FILE_NOT_FOUND;
                }

            } else {
                if (config.getBoolean(ParserTags.HELP)) {
                    System.out.println();
                    System.out.println(jsap.getHelp());
                    System.out.println();
                    return RESULT_HELP;
                } else if (config.getBoolean(ParserTags.VERSION)) {
                    System.out.println();
                    System.out.println(ParserTags.VERSION_STRING);
                    System.out.println();
                    return RESULT_HELP;
                } else if (config.getBoolean(ParserTags.INIT_IO)) {
                    System.out.println();
                    System.out.println(ParserTags.VERSION_STRING);
                    System.out.println();
                    return RESULT_SETUP;
                } else {
                    System.err.println();
                    System.err.println("Usage: " + jsap.getUsage());
                    System.err.println();
                    return RESULT_WRONG_CMD;
                }
            }
        } catch (JSAPException ex) {
            Logger.getLogger(ArgumentParser.class.getName()).log(Level.SEVERE,
                    "Exception with commandline parser.", ex);
            return RESULT_PARSER_EXCEPTION;
        }
    }
}