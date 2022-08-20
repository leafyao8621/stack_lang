import json
from channels.generic.websocket import AsyncWebsocketConsumer
from django.dispatch.dispatcher import receiver
from asgiref.sync import sync_to_async
from api.models import File

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
        success = True
        try:
            if (action == "compile"):
                message = "compilation successful"
            else:
                message = "Run"
        except:
            success = False
        # Send message to room group
        await self.channel_layer.group_send(
            self.room_group_name,
            {
                "type": "chat_message",
                "user_name": user_name,
                "success": success,
                "message": message
            }
        )

    # Receive message from room group
    async def chat_message(self, event):
        user_name = event["user_name"]
        success = event["success"]
        message = event["message"]
        # Send message to WebSocket
        await self.send(
            text_data=json.dumps({
                "user_name": user_name,
                "success": success,
                "message": message
            })
        )
