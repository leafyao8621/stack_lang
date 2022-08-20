from rest_framework import serializers
from ..models import Example

class ExampleSerializer(serializers.ModelSerializer):
    """Basic Serializer for Example
    """
    class Meta:
        model = Example
        fields = (
            "id",
            "file_name"
        )
