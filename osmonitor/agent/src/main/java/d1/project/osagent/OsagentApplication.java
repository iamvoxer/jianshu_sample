package d1.project.osagent;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

/**
 * @author liuyi
 */
@SpringBootApplication
public class OsagentApplication implements CommandLineRunner {


    public static void main(String[] args) {
        SpringApplication.run(OsagentApplication.class, args);
    }

    @Override
    public void run(String... args) {
    }
}
