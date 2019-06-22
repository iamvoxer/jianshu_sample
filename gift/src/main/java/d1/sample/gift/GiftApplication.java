package d1.sample.gift;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import java.util.Date;

/**
 * @author liuyi
 */
@SpringBootApplication
public class GiftApplication implements CommandLineRunner {

    public static void main(String[] args) {
        SpringApplication.run(GiftApplication.class, args);
    }

    @Override
    public void run(String... args) throws Exception {
        RuleService.Instance().loadRule();

        for (int i = 0; i < 20; i++) {
            RuleService.Instance().earnPoint("login", "wanger", new Date());
            Thread.sleep(1000);
            RuleService.Instance().earnPoint("login", "lisi", new Date());
        }
        RuleService.Instance().printResult();
    }
}
