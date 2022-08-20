from rest_framework import viewsets
from rest_framework.decorators import action
from rest_framework.response import Response
from ..serializers import FileSerializer
from ..models import File, User

class FileView(viewsets.ModelViewSet):
    """View related to File
    """
    serializer_class = FileSerializer
    queryset = File.objects.all()
    @action(detail=False, methods=["GET"])
    def list_files(self, request):
        try:
            user_name = request.GET.get("user_name")
            files = list(
                self.queryset
                    .filter(created_by__user_name=user_name)
                    .values("file_name")
                )
            return Response(data={"success": True, "data": files})
        except:
            return Response(data={"success": False})
    @action(detail=False, methods=["POST"])
    def update_file(self, request):
        try:
            user_name = request.data["user_name"]
            file_name = request.data["file_name"]
            data = request.data["data"]
            id =\
                list(
                    User.objects
                        .all()
                        .filter(user_name=user_name)
                        .values("id")\
                )[0]["id"]
            self.queryset.update_or_create(
                created_by_id=id,
                file_name=file_name
            )
            with open(
                f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                f"src/{file_name}.sl",
                "w") as fout:
                fout.write(data)
            return Response(data={"success": True})
        except:
            return Response(data={"success": False})
    @action(detail=False, methods=["GET"])
    def retrieve_file(self, request):
        try:
            user_name = request.GET.get("user_name")
            file_name = request.GET.get("file_name")
            with open(
                f"/home/leaf/stack_lang_online_ide/users/{user_name}/"
                f"src/{file_name}.sl",
                "r") as fin:
                data = fin.read()
            return Response(data={"success": True, "data": data})
        except:
            return Response(data={"success": False})
