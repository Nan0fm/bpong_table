import 'package:flutter/material.dart';

class TransformingView extends StatefulWidget {
  String imagePath;

  @override
  _TransformingViewState createState() => _TransformingViewState();

  TransformingView({this.imagePath});
}

class _TransformingViewState extends State<TransformingView> {
  Color currentColor = Colors.limeAccent;

  double x = -48;
  double y = 0;
  double z = 0;

  @override
  Widget build(BuildContext context) {
    return Transform(
      transform: Matrix4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, z,
        0, 0, 0, 1,
      )..setEntry(3, 2, 0.001)..rotateX(x)..rotateY(y)..rotateZ(z) ,
      alignment: FractionalOffset.center,
      child: GestureDetector(
        onPanUpdate: (details){
          setState(() {
            y = y - details.delta.dx / 100;
            z = z + details.delta.dy / 100;
          });
        },
        child: Image.asset(
         widget.imagePath,
          fit: BoxFit.cover,
          width: 240,
          height: 320,
        ),
      ),
    );
  }
}
