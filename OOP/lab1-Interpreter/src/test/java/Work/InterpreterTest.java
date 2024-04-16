package Work;

import Work.Operators.*;
import Work.Source.*;
import Work.Utilities.*;
import org.junit.Assert;
import org.junit.Test;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static java.lang.System.lineSeparator;
import static org.junit.Assert.*;

public class InterpreterTest {

    @Test
    public void FactoryTest() {
        try {
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{"+","Work.Operators.OperatorPlus"}));
            Factory factory = new Factory(sao);
            IOperator unit = factory.create('+');
            boolean expected = unit instanceof OperatorPlus;
            boolean actual = true;
            assertEquals(expected, actual);
        } catch (ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            ArrayList <SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{"[", "Work.Operators.OperatorOpenBracket"}));
            sao.add(new SymbolAndOperator(new String[]{"[", "Work.Operators.OperatorCloseBracket"}));
            new Factory(sao);
        } catch (ExecOrConfigException e) {
            assertEquals("invalid config: using similar symbols", e.getMessage());
        }

        try {
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{"[", "Work.Operators.OperatorOpenBracket"}));
            sao.add(new SymbolAndOperator(new String[]{"]", "Work.Operators.OperatorOpenBracket"}));
            new Factory(sao);
        } catch (ExecOrConfigException e) {
            assertEquals("invalid config: using similar Classes", e.getMessage());
        }

        try {
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{"+", "Work.Operators.OperatorPlus"}));
            Factory factory = new Factory(sao);
            factory.create('a');
            fail("Expected exception");
        } catch (ExecOrConfigException e) {
            assertEquals("invalid Executable file: uncorrected symbols", e.getMessage());
        }
    }

    @Test
    public void ParseInputTest() {
        try {
            File tmpExec = File.createTempFile("tmpExec", ".txt", new File("."));
            File tmpConf = File.createTempFile("tmpConf", ".txt", new File("."));
            tmpExec.deleteOnExit();
            tmpConf.deleteOnExit();
            String[] actual = {tmpExec.getName(), tmpConf.getName()};
            String[] expected = ParsInput.pars(actual);
            assertEquals(expected, actual);
        } catch (IOException | ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpExec = File.createTempFile("tmpExec", ".txt", new File("."));
            File tmpConf = File.createTempFile("tmpConf", ".txt", new File("."));
            tmpExec.deleteOnExit();
            tmpConf.deleteOnExit();
            String[] actual = {tmpExec.getName(), tmpConf.getName()};
            String[] path = {tmpExec.getName()};
            ByteArrayInputStream input = new ByteArrayInputStream(tmpConf.getName().getBytes());
            System.setIn(input);
            String[] expected = ParsInput.pars(path);
            assertEquals(expected, actual);
            System.setIn(System.in);
        } catch (IOException | ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpExec = File.createTempFile("tmpExec", ".txt", new File("."));
            File tmpConf = File.createTempFile("tmpConf", ".txt", new File("."));
            tmpExec.deleteOnExit();
            tmpConf.deleteOnExit();
            String[] actual = {tmpExec.getName(), tmpConf.getName()};
            String[] path = new String[0];
            ByteArrayInputStream input = new ByteArrayInputStream((tmpExec.getName() + '\n' + tmpConf.getName()).getBytes());
            System.setIn(input);
            String[] expected = ParsInput.pars(path);
            assertEquals(expected, actual);
            System.setIn(System.in);
        } catch (IOException | ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            String[] test = {"a", "b", "c"};
            ParsInput.pars(test);
            fail("exception expected");
        } catch (ExecOrConfigException e) {
            assertEquals("invalid input", e.getMessage());
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpConf = File.createTempFile("tmpConf", ".txt", new File("."));
            tmpConf.deleteOnExit();
            String[] test = {"0", tmpConf.getName()};
            ParsInput.pars(test);
            fail("exception expected");
        } catch (IOException e) {
            assertEquals("executable file was not found", e.getMessage());
        } catch (ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpConf = File.createTempFile("tmpConf", ".txt", new File("."));
            tmpConf.deleteOnExit();
            String[] test = {tmpConf.getName(), "0"};
            ParsInput.pars(test);
            fail("exception expected");
        } catch (IOException e) {
            assertEquals("config file was not found", e.getMessage());
        } catch (ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            String[] test = {"sdf"};
            ParsInput.pars(test);
            fail("exception expected");
        } catch (IOException e) {
            assertEquals("executable file was not found", e.getMessage());
        } catch (ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpExec = File.createTempFile("tmpExec", ".txt", new File("."));
            tmpExec.deleteOnExit();
            String[] test = {tmpExec.getName()};
            ByteArrayInputStream input = new ByteArrayInputStream("0".getBytes());
            System.setIn(input);
            ParsInput.pars(test);
            System.setIn(System.in);
            fail("exception expected");
        } catch (IOException e) {
            assertEquals("config file was not found", e.getMessage());
        } catch (ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpConf = File.createTempFile("tmpConf", ".txt", new File("."));
            tmpConf.deleteOnExit();
            String[] path = new String[0];
            ByteArrayInputStream input = new ByteArrayInputStream(("0" + '\n' + tmpConf.getName()).getBytes());
            System.setIn(input);
            ParsInput.pars(path);
            System.setIn(System.in);
            fail("exception expected");
        } catch (IOException e) {
            assertEquals("executable file was not found", e.getMessage());
        } catch (ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpExec = File.createTempFile("tmpExec", ".txt", new File("."));
            tmpExec.deleteOnExit();
            String[] path = new String[0];
            ByteArrayInputStream input = new ByteArrayInputStream((tmpExec.getName() + '\n' + "0").getBytes());
            System.setIn(input);
            ParsInput.pars(path);
            System.setIn(System.in);
            fail("exception expected");
        } catch (IOException e) {
            assertEquals("config file was not found", e.getMessage());
        } catch (ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        {
            ByteArrayOutputStream output = new ByteArrayOutputStream();
            System.setOut(new PrintStream(output));
            ParsInput.printHelp();
            String expected = output.toString();
            String actual = lineSeparator() + "Start of Program: executable file config file" + lineSeparator()
                    + "Names of Operator Classes:" + lineSeparator() + "Work.Operators.OperatorLeft = ptr--"
                    + lineSeparator() + "Work.Operators.OperatorRight = ptr++" + lineSeparator() + "Work.Operators.OperatorMinus = *ptr--"
                    + lineSeparator() + "Work.Operators.OperatorPlus = *ptr++" + lineSeparator() + "Work.Operators.OperatorRead = read"
                    + lineSeparator() + "Work.Operators.OperatorWrite = write"
                    + lineSeparator() + "Format of config: operator symbol (space)->(space) operator class"
                    + lineSeparator() + "Format of executable file: operator symbols" + lineSeparator() + lineSeparator();
            assertEquals(expected, actual);
            System.setOut(System.out);
        }

        try {
            File tmpExec = File.createTempFile("tmpExec", ".txt", new File("."));
            tmpExec.deleteOnExit();
            PrintWriter pw = new PrintWriter(tmpExec);
            pw.print("> < +");
            pw.close();
            ArrayList<Character> expected = ParsInput.readExec(tmpExec.getName());
            ArrayList<Character> actual = new ArrayList<>(Arrays.asList('>', '<', '+'));
            assertEquals(expected, actual);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpConf = File.createTempFile("tmpConf", ".txt", new File("."));
            tmpConf.deleteOnExit();
            PrintWriter pw = new PrintWriter(tmpConf);
            pw.print("> -> Work.Operators.OperatorRight");
            pw.close();
            ArrayList<SymbolAndOperator> expected = ParsInput.readConfig(tmpConf.getName());
            ArrayList<String[]> actual = new ArrayList<>();
            String[] tmp = {">", "Work.Operators.OperatorRight"};
            actual.add(tmp);
            assertTrue(expected.get(0).getSymbol().equals(actual.get(0)[0].charAt(0)) && expected.get(0).getOperator().equals(actual.get(0)[1]));
        } catch (IOException | ExecOrConfigException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpConf = File.createTempFile("tmpConf", ".txt",new File("."));
            tmpConf.deleteOnExit();
            PrintWriter pw = new PrintWriter(tmpConf);
            pw.print(">");
            pw.close();
            ParsInput.readConfig(tmpConf.getName());
            fail("exception expected");
        } catch (IOException e) {
            throw new RuntimeException(e);
        } catch (ExecOrConfigException e) {
            assertEquals("invalid config: invalid format", e.getMessage());
        }
    }

    @Test
    public void operatorRightTest() {
        try {
            ArrayList<Character> code = new ArrayList<>(List.of('>'));
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{">","Work.Operators.OperatorRight"}));
            Context entity = new Context(code, sao);
            Factory factory = new Factory(sao);
            OperatorPlus plus = new OperatorPlus();
            plus.operation(entity);
            IOperator operator = factory.create(entity.getOperation());
            operator.operation(entity);
            int expected = entity.getByte();
            int actual = 0;
            assertEquals(expected, actual);
        } catch (ExecOrConfigException | IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    public void operatorPlusTest() {
        try {
            ArrayList<Character> code = new ArrayList<>(List.of('+'));
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{"+", "Work.Operators.OperatorPlus"}));
            Context entity = new Context(code, sao);
            Factory factory = new Factory(sao);
            IOperator operator = factory.create(entity.getOperation());
            operator.operation(entity);
            byte expected = entity.getByte();
            byte actual = 0;
            actual++;
            assertEquals(expected, actual);
        } catch (ExecOrConfigException | IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    public void operatorMinusTest() {
        try {
            ArrayList<Character> code = new ArrayList<>(List.of('-'));
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{"-", "Work.Operators.OperatorMinus"}));
            Context entity = new Context(code, sao);
            Factory factory = new Factory(sao);
            IOperator operator = factory.create(entity.getOperation());
            operator.operation(entity);
            byte expected = entity.getByte();
            byte actual = 0;
            actual--;
            assertEquals(expected, actual);
        } catch (ExecOrConfigException | IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    public void operatorLeftTest() {
        try {
            ArrayList<Character> code = new ArrayList<>(List.of('<'));
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{"<", "Work.Operators.OperatorLeft"}));
            Context entity = new Context(code, sao);
            Factory factory = new Factory(sao);
            OperatorPlus plus = new OperatorPlus();
            plus.operation(entity);
            OperatorRight or = new OperatorRight();
            or.operation(entity);
            IOperator operator = factory.create(entity.getOperation());
            operator.operation(entity);
            int expected = entity.getByte();
            int actual = 1;
            assertEquals(expected, actual);
        } catch (ExecOrConfigException | IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    public void operatorWriteTest() {
        try {
            ByteArrayOutputStream output = new ByteArrayOutputStream();
            System.setOut(new PrintStream(output));
            ArrayList<Character> code = new ArrayList<>(List.of(','));
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{",", "Work.Operators.OperatorWrite"}));
            Context entity = new Context(code, sao);
            Factory factory = new Factory(sao);
            IOperator operator = factory.create(entity.getOperation());
            operator.operation(entity);
            String expected = String.valueOf((char) entity.getByte());
            String actual = output.toString();
            Assert.assertEquals(expected, actual);
            System.setOut(System.out);
        } catch (ExecOrConfigException | IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    public void operatorReadTest() {
        try {
            ByteArrayInputStream input = new ByteArrayInputStream("A".getBytes());
            System.setIn(input);
            ArrayList<Character> code = new ArrayList<>(List.of('.'));
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{".", "Work.Operators.OperatorRead"}));
            Context entity = new Context(code, sao);
            Factory factory = new Factory(sao);
            IOperator operator = factory.create(entity.getOperation());
            operator.operation(entity);
            byte expected = entity.getByte();
            byte actual = (byte) 'A';
            Assert.assertEquals(expected, actual);
            System.setIn(System.in);
        } catch (ExecOrConfigException e) {
            assertNotEquals("", e.getMessage());
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    public void operatorOpenBracket() {
        try {
            ArrayList<Character> code = new ArrayList<>(Arrays.asList('[', '[', '+', ']', ']'));
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{"[", "Work.Operators.OperatorOpenBracket"}));
            sao.add(new SymbolAndOperator(new String[]{"]", "Work.Operators.OperatorCloseBracket"}));
            sao.add(new SymbolAndOperator(new String[]{"+", "Work.Operators.OperatorPlus"}));
            Context entity = new Context(code, sao);
            Factory factory = new Factory(sao);
            OperatorPlus plus = new OperatorPlus();
            IOperator operator = factory.create(entity.getOperation());
            operator.operation(entity);
            char expected = entity.getOperation();
            int actual = ']';
            assertEquals(expected, actual);

            plus.operation(entity);
            operator = factory.create(entity.getOperation());
            operator.operation(entity);
            int expected1 = entity.getByte();
            int actual1 = 1;
            assertEquals(expected1, actual1);
        } catch (ExecOrConfigException | IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    public void operatorCloseBracket() {
        try {
            ArrayList<Character> code = new ArrayList<>(Arrays.asList('[', '[', '+', ']', ']'));
            ArrayList<SymbolAndOperator> sao = new ArrayList<>();
            sao.add(new SymbolAndOperator(new String[]{"[", "Work.Operators.OperatorOpenBracket"}));
            sao.add(new SymbolAndOperator(new String[]{"]", "Work.Operators.OperatorCloseBracket"}));
            sao.add(new SymbolAndOperator(new String[]{"+", "Work.Operators.OperatorPlus"}));
            Context entity = new Context(code, sao);
            Factory factory = new Factory(sao);
            for (int i = 0; i < 4; i++) {
                entity.rightCodePtr();
            }
            IOperator operator = factory.create(entity.getOperation());
            operator.operation(entity);
            char expected = entity.getOperation();
            char actual = ']';
            assertEquals(expected, actual);

            OperatorPlus plus = new OperatorPlus();
            plus.operation(entity);
            operator = factory.create(entity.getOperation());
            operator.operation(entity);
            expected = entity.getOperation();
            actual = '[';
            assertEquals(expected, actual);
        } catch (ExecOrConfigException | IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    public void interpretationTest() {
        try {
            File tmpExec = File.createTempFile("tmpExec", ".txt", new File("."));
            File tmpConf = File.createTempFile("tmpConf", ".txt", new File("."));
            tmpExec.deleteOnExit();
            tmpConf.deleteOnExit();
            PrintWriter pw = new PrintWriter(tmpExec);
            pw.print("++++++++++[>+>+++>+++++++>++++++++++<<<<-]>>>++.>+.+++++++..+++.<<++.>+++++++++++++++.>.+++.------.--------.<<+.<.");
            pw.close();
            pw = new PrintWriter(tmpConf);
            pw.println("+ -> Work.Operators.OperatorPlus");
            pw.println("- -> Work.Operators.OperatorMinus");
            pw.println("> -> Work.Operators.OperatorRight");
            pw.println("< -> Work.Operators.OperatorLeft");
            pw.println("[ -> Work.Operators.OperatorOpenBracket");
            pw.println("] -> Work.Operators.OperatorCloseBracket");
            pw.print(". -> Work.Operators.OperatorWrite");
            pw.close();
            String[] files = {tmpExec.getName(), tmpConf.getName()};
            ByteArrayOutputStream output = new ByteArrayOutputStream();
            System.setOut(new PrintStream(output));
            Main.main(files);
            String expected = output.toString();
            String actual = "Hello World!\n";
            assertEquals(expected, actual);
            System.setOut(System.out);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        try {
            File tmpExec = File.createTempFile("tmpExec", ".txt", new File("."));
            File tmpConf = File.createTempFile("tmpConf", ".txt", new File("."));
            tmpExec.deleteOnExit();
            tmpConf.deleteOnExit();
            PrintWriter pw = new PrintWriter(tmpConf);
            pw.println("+ -> Class does not exist");
            pw.close();
            pw = new PrintWriter(tmpExec);
            pw.println("+");
            pw.close();
            String[] files = {tmpExec.getName(), tmpConf.getName()};
            ByteArrayOutputStream output = new ByteArrayOutputStream();
            System.setOut(new PrintStream(output));
            Main.main(files);
            String expected = output.toString();
            String actual = "invalid config: this class doesn't exist" + lineSeparator() + lineSeparator() +
                    "Start of Program: executable file config file" + lineSeparator() +
                    "Names of Operator Classes:" + lineSeparator() +
                    "Work.Operators.OperatorLeft = ptr--" + lineSeparator() +
                    "Work.Operators.OperatorRight = ptr++" + lineSeparator() +
                    "Work.Operators.OperatorMinus = *ptr--" + lineSeparator() +
                    "Work.Operators.OperatorPlus = *ptr++" + lineSeparator() +
                    "Work.Operators.OperatorRead = read" + lineSeparator() +
                    "Work.Operators.OperatorWrite = write" + lineSeparator() +
                    "Format of config: operator symbol (space)->(space) operator class" + lineSeparator() +
                    "Format of executable file: operator symbols" + lineSeparator() + lineSeparator();
            assertEquals(expected, actual);
            System.setOut(System.out);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}