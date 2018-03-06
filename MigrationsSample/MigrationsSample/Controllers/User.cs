using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace MigrationsSample.Controllers
{
    public class User
    {
        public string Id { get; set; }
        public string Name { get; set; }
        //一开始先注释调Age这个属性，用于测试模型变化后的数据库迁移
        public string Age { get; set; }
    }
}
