from rest_framework import viewsets
from rest_framework.decorators import action
from rest_framework.response import Response
from ..serializers import ExampleSerializer
from ..models import Example

class ExampleView(viewsets.ModelViewSet):
    """View related to Example
    """
    serializer_class = ExampleSerializer
    queryset = Example.objects.all()
    @action(detail=False, methods=["GET"])
    def list_examples(self, request):
        try:
            res = list(self.queryset.values("file_name"))
            return Response(data={"success": True, "data": res})
        except:
            return Response(data={"success": False})
    @action(detail=False, methods=["GET"])
    def retrieve_example(self, request):
        try:
            file_name = request.GET.get("file_name")
            with open(
                f"/home/leaf/stack_lang_online_ide/examples/"
                f"src/{file_name}.sl",
                "r") as fin:
                data = fin.read()
            with open(
                f"/home/leaf/stack_lang_online_ide/examples/"
                f"in/{file_name}",
                "r") as fin:
                inp = fin.read()
            return Response(data={"success": True, "data": data, "input": inp})
        except:
            return Response(data={"success": False})
