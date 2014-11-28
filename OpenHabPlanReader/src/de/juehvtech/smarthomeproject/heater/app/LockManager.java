/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.app;

import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Jens
 */
public class LockManager {

    private static String LOCK_FILE_NAME = "heating-manager.lock";
    private static String PATH_TO_LOCK_FILE = "./";
    private static LockManager INSTANCE = null;
    private File lockFile;

    private LockManager() {
        // Check Strings
        if (PATH_TO_LOCK_FILE.contains("\\")) {
            throw new IllegalAccessError("Please use java style for the path "
                    + "(use \"/\" not \"\\\"): " + PATH_TO_LOCK_FILE);
        } else if (!PATH_TO_LOCK_FILE.endsWith("/")) {
            throw new IllegalAccessError("Given path does not end with \"/\": "
                    + PATH_TO_LOCK_FILE);
        } else if (!LOCK_FILE_NAME.endsWith(".lock")) {
            throw new IllegalAccessError("Lock file should end with \".lock\": "
                    + LOCK_FILE_NAME);
        } else if (LOCK_FILE_NAME.contains("/")
                || LOCK_FILE_NAME.contains("\\")) {
            throw new IllegalAccessError("File name looks like a Path: "
                    + LOCK_FILE_NAME);
        }
        // Check path
        File lockPath = new File(PATH_TO_LOCK_FILE);
        if (!lockPath.exists()) {
            throw new IllegalAccessError("Given path does not exist: "
                    + PATH_TO_LOCK_FILE);
        } else if (!lockPath.isDirectory()) {
            throw new IllegalAccessError("Given path is no directory: "
                    + PATH_TO_LOCK_FILE);
        } else if (!lockPath.canWrite()) {
            throw new IllegalAccessError("Given path is not writeable: "
                    + PATH_TO_LOCK_FILE);
        }
        // create file
        lockFile = new File(PATH_TO_LOCK_FILE + LOCK_FILE_NAME);
    }

    public static synchronized LockManager getInstance() {
        if (INSTANCE == null) {
            INSTANCE = new LockManager();
        }
        return INSTANCE;
    }

    public static String getLockFileName() {
        return LOCK_FILE_NAME;
    }

    public static synchronized void setLockFileName(String fileName) {
        if (INSTANCE != null) {
            throw new IllegalAccessError(
                    "Do not set the file name after instance call.");
        }
        LockManager.LOCK_FILE_NAME = fileName;
    }

    public static String getPathToLockFile() {
        return PATH_TO_LOCK_FILE;
    }

    public static synchronized void setPathToLockFile(String path) {
        if (INSTANCE != null) {
            throw new IllegalAccessError(
                    "Do not set the path after instance call.");
        }
        LockManager.PATH_TO_LOCK_FILE = path;
    }

    public synchronized boolean takeLock() {
        try {
            boolean returnvalue = lockFile.createNewFile();
            if (returnvalue) {
                // create shutdown hook
                Runtime.getRuntime().addShutdownHook(new Thread() {
                    @Override
                    public void run() {
                        if (LockManager.getInstance().isLocked()) {
                            Logger.getLogger(LockManager.class.getName())
                                    .warning("Release lock at shotdown.");
                            LockManager.getInstance().releaseLock();
                        }
                    }
                });
            }
            return returnvalue;
        } catch (IOException ex) {
            Logger.getLogger(LockManager.class.getName())
                    .log(Level.SEVERE, "Error while creating Lock", ex);
            return false;
        }
    }

    public synchronized void releaseLock() {
        lockFile.delete();
    }

    public synchronized boolean isLocked() {
        return lockFile.exists();
    }
}
