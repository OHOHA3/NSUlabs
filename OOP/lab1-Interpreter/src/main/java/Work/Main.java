package Work;

import Work.Source.Interpreter;
import Work.Utilities.ExecOrConfigException;
import Work.Utilities.ParsInput;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;


public class Main {
    private static final Logger logger = LogManager.getLogger(Main.class);

    public static void main(String[] args) {
        logger.info("Program has started");
        int start = 0;
        try {
            String[] files;
            files = ParsInput.pars(args);
            Interpreter interpreter = new Interpreter();
            interpreter.interpretation(ParsInput.readExec(files[0]), ParsInput.readConfig(files[1]));
            start++;
        } catch (ExecOrConfigException | IOException e) {
            System.out.println(e.getMessage());
        } finally {
            if (start == 0) {
                ParsInput.printHelp();
            }
        }
        logger.info("Program has ended");
    }
}