from rest_framework import serializers
from ..models import File

class FileSerializer(serializers.ModelSerializer):
    """Basic Serializer for File
    """
    class Meta:
        model = File
        fields = (
            "id",
            "created_by",
            "file_name",
            "compiled"
        )
