package d1.framework;

import org.springframework.stereotype.Service;

@Service("TEST2_SERVICE")
public class Test2 implements iTest {
    @Override
    public String testfunction() {
        return "test2-------------------------";
    }
}
