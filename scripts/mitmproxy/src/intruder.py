from mitmproxy import ctx, http, command  
from typing import Sequence  
import copy  
  
class Intruder:  
    def __init__(self):  
        self.payloads = []  
        self.target_param = ""  
      
    def load(self, loader):  
        loader.add_option(  
            "intruder_wordlist",  
            str,  
            "",  
            "Wordlist of payloads to test"  
        )  
        loader.add_option(  
            "intruder_param",  
            str,  
            "",  
            "Parameter name to replace (e.g., 'password', 'username')"  
        )  
      
    def configure(self, updated):  
        if "intruder_payloads" in updated:  
            if ctx.options.intruder_wordlist:  
                with open(intruder_wordlist, 'r') as f:  
                    self.payloads = [line.strip() for line in f.readlines()]
          
        if "intruder_param" in updated:  
            self.target_param = ctx.options.intruder_param  
      
    @command.command("intruder.attack")  
    def attack_flow(self, flows: Sequence[http.HTTPFlow]) -> None:  
        """对选中的流进行参数爆破"""  
        if not self.payloads or not self.target_param:  
            ctx.log.error("Missing parameters: intruder_payloads intruder_param")  
            return  
          
        for flow in flows:  
            original_flow = flow  
            ctx.log.info(f"start intruder ,target: {self.target_param}")  
              
            for payload in self.payloads:  
                # 复制原始请求  
                new_flow = copy.deepcopy(original_flow)  
                  
                # 修改参数值  
                if self.target_param in new_flow.request.query:  
                    new_flow.request.query[self.target_param] = payload  
                elif self.target_param in new_flow.request.urlencoded_form:  
                    new_flow.request.urlencoded_form[self.target_param] = payload  
                  
                # 重放请求  
                ctx.master.commands.call("replay.client", [new_flow])  
                ctx.log.info(f"send payload: {payload}")  
  
addons = [Intruder()]
