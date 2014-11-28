/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.juehvtech.smarthomeproject.heater.heatingplan;

import com.csvreader.CsvReader;
import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Jens
 */
public class HeatingPlanReader {

    private static final Logger LOG = Logger.getLogger(HeatingPlanReader.class.getName());
    private final String pathToFile;

    public HeatingPlanReader(String pathToFile) {
        File planFile = new File(pathToFile);
        if (!planFile.exists()
                || planFile.isDirectory()
                || !planFile.canRead()) {
            LOG.severe("Plan is not accessable! Exit.");
            System.exit(-1);
        }

        this.pathToFile = pathToFile;

    }

    public HeatingPlan read() {
        try {
            List<String[]> lines = new ArrayList<>();
            CsvReader reader = new CsvReader(pathToFile, ',',
                    Charset.forName("UTF-8"));

            long startTime = System.currentTimeMillis();
            while (reader.readRecord()) {
                // copy csv file to ram
                lines.add(reader.getValues());
            }
            long endTime = System.currentTimeMillis();
            LOG.log(Level.INFO, "finished reading csv file in {0}ms.", (endTime - startTime));

            reader.close();


            List<List<Float>> plan = new ArrayList<>();
            for (String[] line : lines) {
                if (line.length > 0) {
                    if (line[0].equalsIgnoreCase("#")) {
                        continue;
                    }
                    List<Float> lineList = new ArrayList<>();
                    for (int i = 1; i < line.length; i++) {
                        lineList.add(Float.parseFloat(line[i]));
                    }
                    plan.add(lineList);
                }
            }

            return new HeatingPlan(plan);
        } catch (IOException ex) {
            Logger.getLogger(HeatingPlanReader.class.getName())
                    .log(Level.SEVERE, "THIS SHOULD NOT HAPPEN", ex);
            return null;
        }
    }
}
