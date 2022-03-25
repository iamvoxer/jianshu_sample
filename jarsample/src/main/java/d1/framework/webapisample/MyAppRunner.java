package d1.framework.webapisample;

import d1.framework.iTest;
import org.springframework.boot.ApplicationArguments;
import org.springframework.boot.ApplicationRunner;
import org.springframework.stereotype.Component;

/**
 * @author Buter
 * @date 2020/3/20 14:59
 */
@Component
public class MyAppRunner implements ApplicationRunner {
    private final TestContext context;
    public MyAppRunner(TestContext context) {
        this.context = context;
    }

    @Override
    public void run(ApplicationArguments args) {
        iTest test1 = this.context.getiTest("TEST1_SERVICE");
        System.out.println(test1.testfunction());
        iTest test2 = this.context.getiTest("TEST2_SERVICE");
        System.out.println(test2.testfunction());
    }
}
