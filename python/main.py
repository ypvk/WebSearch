import sys
from PySide.QtGui import QApplication

def main(argv):
   app = QApplication(argv)
   mainwindow = MainWindow()
   sys.exit(app.exec_())
if __name__ == "__main__":
    main(sys.argv)
