package d1.test.service;

import org.springframework.web.bind.annotation.*;

import java.util.Hashtable;
import java.util.Map;

@RestController
@RequestMapping("/test")
public class TestController {
    @RequestMapping(value = "/get1", method = RequestMethod.GET)
    public String get1(String para) {
        return "get1:" + para;
    }

    @RequestMapping(value = "/get2/{id}", method = RequestMethod.GET)
    public String get2(@PathVariable Integer id) {
        return "get2:" + id;
    }

    @RequestMapping(value = "/get3/{id}", method = RequestMethod.GET)
    public Map<String, Object> get3(@PathVariable Integer id) {
        Map<String, Object> result = new Hashtable<>();
        result.put("name", "myname");
        result.put("sex", id);
        return result;
    }

    @RequestMapping(value = "/get4", method = RequestMethod.GET)
    public String get4(@RequestHeader(name = "token") String token) {
        return "get4:" + token;
    }

    @RequestMapping(value = "/post1", method = RequestMethod.POST)
    public String post1(@RequestBody TestModel model) {
        return "post1:" + model.toString();
    }

    @RequestMapping(value = "/post2", method = RequestMethod.POST)
    public String post2(@RequestBody TestModel model, @RequestHeader(name = "token") String token) {
        return "post2:" + model.toString() + token;
    }

    @RequestMapping(value = "/put1", method = RequestMethod.PUT)
    public String put1(@RequestBody TestModel model, @RequestHeader(name = "token") String token) {
        return "put1:" + model.toString() + token;
    }

    @RequestMapping(value = "/del1/{id}", method = RequestMethod.DELETE)
    public String del1(@PathVariable Integer id) {
        return "del1:" + id;
    }
}
