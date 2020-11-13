<ul> {
for $x in doc("cafe8.xml")/catalog/array
where $x/ObjectType = "сезонное кафе"
return <li>{data($x/StationaryObjectName)} - {data($x/ObjectType)}</li>
} </ul>