open Belt;

let str = React.string;

let rem_of_px = pixel => (pixel->float_of_int /. 16.)->Css.rem;

let createMarkup = str => <span dangerouslySetInnerHTML={"__html": str} />;

let unknownErrorMessage = "An unknown error has occurred.";

let mergeDeep = (items: array(array('a))) =>
  items->Belt.Array.reduce([||], (acc, item) =>
    acc->Belt.Array.concat(item)
  );

let arrayKeepSome = array =>
  array->Array.reduce([||], (acc, item) =>
    switch (item) {
    | None => acc
    | Some(item) => Array.concat(acc, [|item|])
    }
  );

module Input = {
  let handleEnterPressOnKeyDown = (event, callback) => {
    ReactEvent.Synthetic.persist(event);
    if (event->ReactEvent.Keyboard.which == 13
        || event->ReactEvent.Keyboard.keyCode == 13) {
      callback();
    };
  };

  let handleArrowUpPressOnKeyDown = (event, callback) => {
    ReactEvent.Synthetic.persist(event);
    if (event->ReactEvent.Keyboard.which == 38
        || event->ReactEvent.Keyboard.keyCode == 38) {
      callback();
    };
  };

  let handleArrowDownPressOnKeyDown = (event, callback) => {
    ReactEvent.Synthetic.persist(event);
    if (event->ReactEvent.Keyboard.which == 40
        || event->ReactEvent.Keyboard.keyCode == 40) {
      callback();
    };
  };

  let focus = node => {
    node->ReactDOMRe.domElementToObj##focus();
  };

  let focusOn = id => {
    switch (Document.getElementById(id)) {
    | None =>
      switch (Document.getElementById("default-container")) {
      | None => ()
      | Some(node) => node->focus
      }
    | Some(node) => node->focus
    };
  };

  let boolChangeWrapper = (handleChange, event) =>
    ReactEvent.Form.target(event)##checked->handleChange->ignore;

  let downloadFile: (string, string, string, string) => unit = [%raw
    {|
  // Resource: https://stackoverflow.com/questions/3975648/how-to-set-content-disposition-attachment-via-javascript

	async function(fileData, fileName, dataType, blobType) {
		const blob = new Blob([fileData],{type: blobType, charset: "UTF-8"});

    if (window.navigator && window.navigator.msSaveOrOpenBlob) {
      window.navigator.msSaveOrOpenBlob(blob);
      return;
    }

    const data = await window.URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.dataType = dataType;
    link.href = data;
    link.download = fileName;
    link.dispatchEvent(new MouseEvent('click'));

    setTimeout(function () {
      // For Firefox it is necessary to delay revoking the ObjectURL
      window.URL.revokeObjectURL(data)
    }, 60);
	}
|}
  ];
};