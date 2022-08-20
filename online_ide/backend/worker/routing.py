from django.urls import re_path

from . import consumers

websocket_urlpatterns = [
    re_path(r'ws/worker/(?P<room_name>\w+)/$', consumers.WorkerConsumer.as_asgi()),
]
