package Work.Utilities;

import Work.Source.SymbolAndOperator;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class ParsInput {
    private static final Logger logger = LogManager.getLogger(ParsInput.class);

    public static void printHelp() {
        System.out.println();
        System.out.println("Start of Program: executable file config file");
        System.out.println("Names of Operator Classes:");
        System.out.println("Work.Operators.OperatorLeft = ptr--");
        System.out.println("Work.Operators.OperatorRight = ptr++");
        System.out.println("Work.Operators.OperatorMinus = *ptr--");
        System.out.println("Work.Operators.OperatorPlus = *ptr++");
        System.out.println("Work.Operators.OperatorRead = read");
        System.out.println("Work.Operators.OperatorWrite = write");
        System.out.println("Format of config: operator symbol (space)->(space) operator class");
        System.out.println("Format of executable file: operator symbols");
        System.out.println();
    }

    public static String[] pars(@NotNull String[] input) throws ExecOrConfigException, FileNotFoundException {
        String[] files = new String[2];
        if (input.length > 2) {
            logger.error("invalid input");
            throw new ExecOrConfigException("invalid input");
        }
        if (input.length == 2) {
            File executableFile = new File(input[0]);
            File config = new File(input[1]);
            if (!executableFile.exists()) {
                logger.error("executable file was not found");
                throw new FileNotFoundException("executable file was not found");
            }
            if (!config.exists()) {
                logger.error("config file was not found");
                throw new FileNotFoundException("config file was not found");
            }
            files[0] = input[0];
            files[1] = input[1];
        }
        if (input.length == 1) {
            File executableFile = new File(input[0]);
            if (!executableFile.exists()) {
                logger.error("executable file was not found");
                throw new FileNotFoundException("executable file was not found");
            }
            System.out.println("Please, write full name of config file");
            Scanner scanner = new Scanner(System.in);
            String configName = scanner.nextLine();
            File config = new File(configName);
            if (!config.exists()) {
                logger.error("config file was not found");
                throw new FileNotFoundException("config file was not found");
            }
            files[0] = input[0];
            files[1] = configName;
        }
        if (input.length == 0) {
            System.out.println("Please, write full name of executable file");
            Scanner scanner = new Scanner(System.in);
            String executableName = scanner.nextLine();
            File executableFile = new File(executableName);
            if (!executableFile.exists()) {
                logger.error("executable file was not found");
                throw new FileNotFoundException("executable file was not found");
            }
            System.out.println("Please, write full name of config file");
            String configName = scanner.nextLine();
            File configFile = new File(configName);
            if (!configFile.exists()) {
                logger.error("config file was not found");
                throw new FileNotFoundException("config file was not found");
            }
            files[0] = executableName;
            files[1] = configName;
        }
        return files;
    }

    public static ArrayList<Character> readExec(@NotNull String execFile) throws IOException {
        ArrayList<Character> code = new ArrayList<>();
        File file = new File(execFile);
        FileReader fr = new FileReader(file);
        int c = 0;
        while (c != -1) {
            c = fr.read();
            if (c == '\r' || c == ' ' || c == '\n' || c == -1) {
                continue;
            }
            code.add((char) c);
        }
        fr.close();
        return code;
    }

    public static ArrayList<SymbolAndOperator> readConfig(@NotNull String config) throws IOException, ExecOrConfigException {
        ArrayList<SymbolAndOperator> symbolsAndOperators = new ArrayList<>();
        File file = new File(config);
        String line;
        String[] words;
        try (Scanner scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                line = scanner.nextLine();
                words = line.split(" -> ");
                symbolsAndOperators.add(new SymbolAndOperator(words));
            }
        }
        return symbolsAndOperators;
    }
}

