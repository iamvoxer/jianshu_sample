package d1.sample.feign;

import feign.RequestInterceptor;
import feign.RequestTemplate;
import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.stereotype.Component;
import org.springframework.web.bind.annotation.*;

import java.util.Map;

/**
 * @author liuyi
 */
@FeignClient(
        //服务名
        name = "demo-service",
        //服务地址
        url = "http://localhost:8080/test/",
        fallback = TestService2Fallback.class,
        configuration = FeignInterceptor.class
)
public interface TestService2 {
    @RequestMapping(value = "/get1", method = RequestMethod.GET)
    public String get1(@RequestParam("para") String para);

    @RequestMapping(value = "/get2/{id}", method = RequestMethod.GET)
    public String get2(@RequestParam("id") Integer id);

    @RequestMapping(value = "/get3/{id}", method = RequestMethod.GET)
    public Map<String, Object> get3(@RequestParam("id") Integer id);

    @RequestMapping(value = "/get4", method = RequestMethod.GET)
    public String get4(@RequestHeader(name = "token") String token);

    @RequestMapping(value = "/post1", method = RequestMethod.POST)
    public String post1(@RequestBody TestModel model);

    @RequestMapping(value = "/post2", method = RequestMethod.POST)
    public String post2(@RequestBody TestModel model, @RequestHeader(name = "token") String token);

    @RequestMapping(value = "/put1", method = RequestMethod.PUT)
    public String put1(@RequestBody TestModel model, @RequestHeader(name = "token") String token);

    @RequestMapping(value = "/del1/{id}", method = RequestMethod.DELETE)
    public String del1(@RequestParam("id") Integer id);
}

@Component
class FeignInterceptor implements RequestInterceptor {
    @Override
    public void apply(RequestTemplate requestTemplate) {
        requestTemplate.header("user", "myuser1");
        requestTemplate.header("password", "mypassword");
    }
}

@Component
class TestService2Fallback implements TestService2 {
    @Override
    public String get1(String para) {
        return "default get1 value";
    }

    @Override
    public String get2(Integer id) {
        return "default get2 value ";
    }

    @Override
    public Map<String, Object> get3(Integer id) {
        return null;
    }

    @Override
    public String get4(String token) {
        return null;
    }

    @Override
    public String post1(TestModel model) {
        return null;
    }

    @Override
    public String post2(TestModel model, String token) {
        return null;
    }

    @Override
    public String put1(TestModel model, String token) {
        return null;
    }

    @Override
    public String del1(Integer id) {
        return null;
    }
}