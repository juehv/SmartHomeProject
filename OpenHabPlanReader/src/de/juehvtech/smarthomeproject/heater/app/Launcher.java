/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.app;

import de.juehvtech.smarthomeproject.heater.manager.MainRunner;
import java.util.logging.LogManager;

/**
 *
 * @author Jens
 */
public class Launcher {

    public static void main(String[] args) throws InterruptedException {
        // Parse Arguments
        int result = ArgumentParser.parse(args);

        if (result == ArgumentParser.RESULT_OK) {
            // Disable Logging
            LogManager.getLogManager().reset();
            // Run App
            Thread main = new Thread(new MainRunner());
            main.start();
            main.join();
        } else if (result == ArgumentParser.RESULT_SETUP) {
            // not needed now
        }
    }
}
