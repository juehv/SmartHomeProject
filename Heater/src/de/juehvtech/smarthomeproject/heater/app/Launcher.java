/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.app;

import de.juehvtech.smarthomeproject.heater.manager.MainRunner;

/**
 *
 * @author Jens
 */
public class Launcher {

    public static void main(String[] args) throws InterruptedException {
        // Parse Arguments
        int result = ArgumentParser.parse(args);

        if (result == ArgumentParser.RESULT_OK) {
            // Take Lock
            LockManager.setPathToLockFile(Properties.LOCK_FILE_PATH);
            LockManager.setLockFileName(Properties.LOCK_FILE_NAME);
            LockManager.getInstance().takeLock();

            // Run App
            Thread main = new Thread(new MainRunner());
            main.start();
            main.join();

            // Unlock
            LockManager.getInstance().releaseLock();

        } else if (result == ArgumentParser.RESULT_SETUP) {
            // not needed now
        }
    }
}
