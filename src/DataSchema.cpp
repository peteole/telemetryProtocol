#include "DataSchema.h"
String BasicSensorValue::toJSON(){
    return "{name:"+this->name+", size: "+this->size+", type: "+this->type;
}
String SensorValueList::toJSON(){
    String JSON= "{name:"+this->name+", size: "+this->size+", values:[\n";
    for(int i=0;i<this->length;i++){
        JSON+=this->Sensorvalues[i].toJSON()+",\n";
    }
    JSON+="]\n}";
    return JSON;
}