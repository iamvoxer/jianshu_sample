var ruleService = Java.type("d1.sample.gift.RuleService")

// 账号登录相关规则
// 1. 基本：每登录一次增加1分,一天最多5次
// 2. 特殊活动：10月1号-10月7号期间登录，每登录一次增加2分，一天最多20分
// 3. 特殊：注册时间是2017年的，登录不加分
function login(date, user) {
    //Java.type("java.lang.System").out.println("debug"+date+user);

    //1. 判断注册时间是否是2017年
    if (ruleService.Instance().getRegistYear(user) == "2017") {
        return {point: 0, desc: user + "在" + date + "登录，由于注册时间2017年，所以没有赚取积分"};
    }
    //2. 判断登录时间是否在特殊活动期间
    if (ruleService.Instance().betweenDate(date, "2019-10-01", "2019-10-07")) {
        if (ruleService.Instance().getTodayTotalPoint(user) >= 20) {
            return {point: 0, desc: user + "在" + date + "登录，由于当天赚取积分已达到20分，所以没有赚取积分"};
        }
        return {point: 2, desc: user + "在" + date + "登录，赚取积分2"};
    }
    //3. 正常积分
    if (ruleService.Instance().getTodayTotalPoint(user) >= 5) {
        return {point: 0, desc: user + "在" + date + "登录，由于当天赚取积分已达到5分，所以没有赚取积分"};
    }
    return {point: 1, desc: user + "在" + date + "登录，赚取积分1"};
}
