#pragma once


enum class RoadLaneRoadMarkType;
enum class RoadLaneRoadMarkColor;
enum class RoadLaneRoadMarkLaneChange;
enum class RoadLaneRoadDescriptionType;



class RoadLaneRoadMark
{
public:
    RoadLaneRoadMark(double sOffset, RoadLaneRoadDescriptionType descriptionType, RoadLaneRoadMarkType roadMark, RoadLaneRoadMarkColor color, RoadLaneRoadMarkLaneChange laneChange)
    {
        this->sOffset = sOffset;
        this->roadMark = roadMark;
        this->color = color;
        this->laneChange = laneChange;
        this->descriptionType = descriptionType;
    }


    RoadLaneRoadMarkType getType() {return roadMark;}

    double getSOffset(){ return sOffset;}

    RoadLaneRoadDescriptionType getDescriptionType(){return descriptionType;}


private:

double sOffset;
RoadLaneRoadMarkType roadMark;
RoadLaneRoadMarkColor color;
RoadLaneRoadMarkLaneChange laneChange;
RoadLaneRoadDescriptionType descriptionType;





};



