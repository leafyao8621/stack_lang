import json
import asyncio
import traceback
from channels.generic.websocket import AsyncWebsocketConsumer

class WorkerConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        self.room_name = self.scope["url_route"]["kwargs"]["room_name"]
        self.room_group_name = "worker_%s" % self.room_name

        # Join room group
        await self.channel_layer.group_add(
            self.room_group_name,
            self.channel_name
        )

        await self.accept()

    async def disconnect(self, close_code):
        # Leave room group
        await self.channel_layer.group_discard(
            self.room_group_name,
            self.channel_name
        )

    # Receive message from WebSocket
    async def receive(self, text_data):
        text_data_json = json.loads(text_data)
        user_name = text_data_json["user_name"]
        action = text_data_json["action"]
        file_name = text_data_json["file_name"]
        message = ""
        output = ""
        success = True
        try:
            if (action == "compile"):
                with open(
                    f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                    f"out/{file_name}",
                    "w") as fout:
                    p =\
                        await asyncio.create_subprocess_exec(
                            "slc",
                            f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                            f"src/{file_name}.sl",
                            f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                            f"bin/{file_name}",
                            stdout=fout,
                            stderr=fout
                        )
                    ret = await p.wait()
                if (ret):
                    success = False
                    with open(
                        f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                        f"out/{file_name}",
                        "r") as fin:
                        message = fin.read()
                else:
                    message = "compilation successful"
            elif (action == "run"):
                with\
                    open(
                        f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                        f"out/{file_name}",
                        "w"
                    ) as fout,\
                    open(
                        f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                        f"in/{file_name}",
                        "r"
                    ) as fin:
                    p =\
                        await asyncio.create_subprocess_exec(
                            f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                            f"bin/{file_name}",
                            stdout=fout,
                            stdin=fin,
                            stderr=fout
                        )
                    ret = await asyncio.wait_for(p.communicate(), 10)
                if (ret[0]):
                    success = False
                    message = f"run failed with code {ret}"
                else:
                    message = "run successfull"
                with open(
                    f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                    f"out/{file_name}",
                    "r") as fin:
                    output = fin.read()
            elif (action == "run example"):
                with\
                    open(
                        f"/home/leaf/stack_lang_online_ide/examples/"
                        f"out/{user_name}_{file_name}",
                        "w"
                    ) as fout,\
                    open(
                        f"/home/leaf/stack_lang_online_ide/examples/"
                        f"in/{file_name}",
                        "r"
                    ) as fin:
                    p =\
                        await asyncio.create_subprocess_exec(
                            f"/home/leaf/stack_lang_online_ide/examples/"
                            f"bin/{file_name}",
                            stdout=fout,
                            stdin=fin,
                            stderr=fout
                        )
                    ret = await asyncio.wait_for(p.communicate(), 10)
                if (ret[0]):
                    success = False
                    message = f"run failed with code {ret}"
                else:
                    message = "run successfull"
                with open(
                    f"/home/leaf/stack_lang_online_ide/examples/"
                    f"out/{user_name}_{file_name}",
                    "r") as fin:
                    output = fin.read()
            else:
                message = "unknown action"
                success = False
        except asyncio.exceptions.TimeoutError:
            success = False
            message = "timeout reached"
        except FileNotFoundError:
            success = False
            message =\
                "file not found, please compile"
        except:
            success = False
            message = "unknown error"
            traceback.print_exc()
        # Send message to room group
        await self.channel_layer.group_send(
            self.room_group_name,
            {
                "type": "chat_message",
                "user_name": user_name,
                "success": success,
                "message": message,
                "output": output
            }
        )

    # Receive message from room group
    async def chat_message(self, event):
        user_name = event["user_name"]
        success = event["success"]
        message = event["message"]
        output = event["output"]
        # Send message to WebSocket
        await self.send(
            text_data=json.dumps({
                "user_name": user_name,
                "success": success,
                "message": message,
                "output": output
            })
        )
