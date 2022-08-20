from rest_framework import viewsets
from rest_framework.decorators import action
from rest_framework.response import Response
from ..serializers import UserSerializer
from ..models import User

import os

class UserView(viewsets.ModelViewSet):
    """View related to User
    """
    serializer_class = UserSerializer
    queryset = User.objects.all()
    @action(detail=False, methods=["POST"])
    def login(self, request):
        """Handles login
        Args:
            request ([type]):
                Fields:
                    user_name: user name
        Returns:
            Response: success or not
                Fields:
                    success: boolean indicating success
        """
        try:
            user_name = request.data["user_name"]
            res = len(list(self.queryset.filter(user_name=user_name)))
            if (not res):
                self.queryset.create(user_name=user_name)
                os.mkdir(f"/home/leaf/stack_lang_online_ide/users/{user_name}")
                os.mkdir(
                    f"/home/leaf/stack_lang_online_ide/users/{user_name}/src"
                )
                os.mkdir(
                    f"/home/leaf/stack_lang_online_ide/users/{user_name}/bin"
                )
            return Response(data={"success": True})
        except:
            return Response(data={"success": False})
