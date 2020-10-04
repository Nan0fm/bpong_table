import 'dart:math';

import 'package:flutter/material.dart';
import 'package:flutter_colorpicker/flutter_colorpicker.dart';

class Body extends StatefulWidget {
  @override
  _BodyState createState() => _BodyState();
}

class _BodyState extends State<Body> {
  Color currentBlueColor = Colors.blue;
  Color currentRedColor = Colors.red;

  double x = -48;
  double y = 0;
  double z = 0;

  @override
  Widget build(BuildContext context) {
    Size size = MediaQuery.of(context).size;
    double kdpadding = 20;
    double halfWidth = size.width / 2 - kdpadding * 4;
    return Column(
      // mainAxisAlignment: MainAxisAlignment.center,
      crossAxisAlignment: CrossAxisAlignment.center,
      children: [
        Expanded(
          flex: 2,
          child: Container(
            width: size.width,
            padding: EdgeInsets.symmetric(horizontal: 24, vertical: 20),
            margin: EdgeInsets.symmetric(horizontal: 24, vertical: 20),
            child: buildTransform(
              "assets/images/starts.jpg",
            ),
          ),
        ),
        Wrap(
          runAlignment: WrapAlignment.spaceBetween,
          spacing: 20,
          runSpacing: 8,
          children: [
            Container(
                width: halfWidth,
                child: FlatButton(
                    onPressed: () {
                      rotateTable(true);
                    },
                    child: Text("Rotate Left"))),
            Container(
                width: halfWidth,
                child: FlatButton(
                    onPressed: () {
                      rotateTable(false);
                    },
                    child: Text("Rotate right"))),
            Container(
                width: halfWidth,
                child: FlatButton(
                    onPressed: () {
                      flipTable(false);
                    },
                    child: Text("Vertical"))),
            Container(
                width: halfWidth,
                child: FlatButton(
                    onPressed: () {
                      // rotateTable()
                    },
                    child: Text("Perspective"))),
          ],
        )
      ],
    );
  }

  Transform buildTransform(String imagePath) {
    return Transform(
      transform: Matrix4(
        1,        0,        0,        0,
        0,        cos(y),        -sin(y),        0,
        0,        sin(y),        cos(y),        z,
        0,        0,        0,        1,
      )
        ..setEntry(3, 2, 0.001)
        ..rotateX(x)
        ..rotateY(y)
        ..rotateZ(z),
      alignment: FractionalOffset.center,
      child: GestureDetector(
        onPanUpdate: (details) {
          setState(() {
            y = y - details.delta.dx / 100;
            x = x + details.delta.dy / 100;
          });
        },
        child: Container(
          decoration: BoxDecoration(
              color: Colors.black,
              border: Border.all(color: Colors.white, width: 6),
              borderRadius: BorderRadius.circular(6),
              boxShadow: [
                BoxShadow(
                    color: Colors.black38,
                    offset: Offset(12 * y.sign, 12 * y.sign),
                    blurRadius: 12)
              ]),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              GestureDetector(
                onTap: () {
                  chooseColor(false);
                },
                child: Padding(
                  padding: const EdgeInsets.all(12.0),
                  child: Image.asset(
                    "assets/images/circles_red_little.png",
                    fit: BoxFit.fitWidth,
                    color: currentRedColor,
                  ),
                ),
              ),
              GestureDetector(
                onTap: () {
                  chooseColor(true);
                },
                child: Padding(
                  padding: const EdgeInsets.all(12.0),
                  child: Image.asset(
                    "assets/images/circles_blue_little.png",
                    fit: BoxFit.fitWidth,
                    color: currentBlueColor,
                  ),
                ),
              )
            ],
          ),
        ),
      ),
    );
  }

  void rotateTable(bool toRight) {
    setState(() {
      if (toRight) {
        // x = x + 45*3.14/180;
        z = z + 45 * 3.14 / 180;
      } else {
        // x = x - 45*3.14/180;
        z = z - 45 * 3.14 / 180;
      }
    });
  }

  void flipTable(bool flip) {
    setState(() {
      if (flip) {
        x = -48;
        y = 0;
        z = 0;
      } else {
        x = 0;
        y = 0;
        z = 0;
      }
    });
  }

  void chooseColor(bool isBlue) {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          titlePadding: const EdgeInsets.all(0.0),
          contentPadding: const EdgeInsets.all(0.0),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(25.0),
          ),
          content: SingleChildScrollView(
            child: SlidePicker(
              pickerColor: isBlue ? currentBlueColor : currentRedColor,
              onColorChanged: (color) {
                setState(() {
                  isBlue ? currentBlueColor = color : currentRedColor = color;
                });
              },
              paletteType: PaletteType.rgb,
              enableAlpha: false,
              displayThumbColor: true,
              showLabel: false,
              showIndicator: true,
              indicatorBorderRadius: const BorderRadius.vertical(
                top: const Radius.circular(25.0),
              ),
            ),
          ),
        );
      },
    );
  }
}
