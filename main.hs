import System.Environment (getArgs)
import System.IO (openFile, IOMode(ReadMode), hGetContents)

main = do
        args <- getArgs
        file <- openFile (head args) ReadMode
        text <- hGetContents file
        print text
